#include "VolumeReconstructionPBM.h"

#include <time.h>
#include <vtkMath.h>

vtkSmartPointer<vtkImageData> VolumeReconstructionPBM::generateVolume()
{
    std::cout<<"Generating Volume Data"<<std::endl;
    
    volumeData = vtkSmartPointer<vtkImageData>::New();
    volumeData->SetNumberOfScalarComponents(1);
    volumeData->SetScalarType(VTK_UNSIGNED_CHAR);
    volumeData->SetOrigin(volumeOrigin[0],volumeOrigin[1],volumeOrigin[2]);
    volumeData->SetDimensions(volumeSize[0],volumeSize[1],volumeSize[2]);
    volumeData->SetSpacing(scale[0]*resolution,scale[1]*resolution,scale[1]*resolution);
    volumeData->AllocateScalars();
        
    /** Pointer to voxels who have been filled in the bin filling stage*/
    filledVolume = vtkSmartPointer<vtkImageData>::New();
    filledVolume->SetScalarTypeToUnsignedChar();
    filledVolume->SetNumberOfScalarComponents(1);
    filledVolume->SetOrigin(volumeOrigin[0],volumeOrigin[1],volumeOrigin[2]);
    filledVolume->SetDimensions(volumeSize[0],volumeSize[1],volumeSize[2]);
    filledVolume->SetSpacing(scale[0]*resolution,scale[1]*resolution,scale[1]*resolution);
    filledVolume->AllocateScalars();   

	     //create accumulation buffer
    AccDataVolume = vtkSmartPointer<vtkImageData>::New();
    AccDataVolume->SetScalarTypeToUnsignedChar();
    AccDataVolume->SetNumberOfScalarComponents(1);
    AccDataVolume->SetOrigin(volumeOrigin[0],volumeOrigin[1],volumeOrigin[2]);
    AccDataVolume->SetDimensions(volumeSize[0],volumeSize[1],volumeSize[2]);
    AccDataVolume->SetSpacing(scale[0]*resolution,scale[1]*resolution,scale[1]*resolution);
    AccDataVolume->AllocateScalars();
    
    unsigned char * volumeVoxel;
    unsigned char * filledVoxel;
	unsigned char * accVoxel;
    
    for(int i=0; i<volumeSize[0]; i++){
	for(int j=0; j<volumeSize[1]; j++){
		for(int k=0; k<volumeSize[2]; k++){
                    
                    // get pointer to the current volume voxel   
                    volumeVoxel = static_cast<unsigned char *> (
                            volumeData->GetScalarPointer(i,j,k));
                    filledVoxel = static_cast<unsigned char *> (
                            filledVolume->GetScalarPointer(i,j,k));
					// get pointer to the current volume voxel   
                    accVoxel = static_cast<unsigned char *> (
                            AccDataVolume->GetScalarPointer(i,j,k));                                   
                    
                    volumeVoxel[0] = 0;
                    filledVoxel[0] = 0;
					accVoxel[0] = 0;
                    
                    
                }
        }
    }

    /////////////////////////////////
    this->binFilling();
	this->holeFillingFixedRegion();
  
    return volumeData;

}

void VolumeReconstructionPBM::binFilling()
{

    std::cout<<"Calculating voxel values"<<std::flush;
    clock_t begin = clock();

	vnl_vector<double> point;
	point.set_size(4);
	point[0]=0;
	point[1]=0;
	point[2]=0;
	point[3]=1;

    for (int i = 0; i < volumeImageStack.size(); i++)
    {
        int * imageSize = volumeImageStack.at(i)->GetDimensions();
        std::cout<<"."<<std::flush;
        
        for (int x = 0; x<imageSize[0]; x++){
            for (int y = 0; y<imageSize[1]; y++){
                
				point[0]=scale[0]*x;
				point[1]=scale[1]*y;

				vnl_vector<double> transformedPoint = transformStack.at(i)*point;

                int voxel[3];
                voxel[0] = vtkMath::Floor((transformedPoint[0] - volumeOrigin[0])/(scale[0]*resolution));
                voxel[1] = vtkMath::Floor((transformedPoint[1] - volumeOrigin[1])/(scale[1]*resolution));
                voxel[2] = vtkMath::Floor((transformedPoint[2] - volumeOrigin[2])/(scale[1]*resolution));                
                                                                  
                unsigned char * imagePixel = static_cast<unsigned char *> (
                        volumeImageStack.at(i)->GetScalarPointer(x, y, 0));
                

                // get pointer to the current volume voxel 
                unsigned char * volumeVoxel = static_cast<unsigned char *> (
                        volumeData->GetScalarPointer(voxel[0], voxel[1], voxel[2]));

                // get pointer to the current accumator volume voxel 
                unsigned char * accDataVoxel = static_cast<unsigned char *> (
                        AccDataVolume->GetScalarPointer(voxel[0], voxel[1], voxel[2]));


                // get pointer to the current fill volume voxel 
                unsigned char * fillVoxel = static_cast<unsigned char *> (
                        filledVolume->GetScalarPointer(voxel[0], voxel[1], voxel[2]));


                // set voxel value with the corresponding pixel value
				if (fillVoxel[0] == 0)
					fillVoxel[0] = 1;

				accDataVoxel[0]++;

				float temp = volumeVoxel[0] + (imagePixel[0]-volumeVoxel[0])/accDataVoxel[0];
				volumeVoxel[0] = (unsigned char) temp;
				
          }
        }                
    
     }

    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks * 10) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed reconstructing volume: "<< double(diffms)<<" ms" <<std::end;

}

void VolumeReconstructionPBM::holeFillingFixedRegion()
{

	int kernelSize = 11;
	int windowStep = vtkMath::Floor(kernelSize/2);
    
    double maxDistance = calcMaxDistance();
    std::cout<<"Filling Empty Voxels with a max Kernel of "<<kernelSize<<"x"<<kernelSize<<std::endl;
    
    clock_t begin = clock();

    // create volume pointers
    unsigned char * filledVoxel;
    unsigned char * innerVoxel;

    for(int x=0; x<volumeSize[0]; x++){
        
	std::cout<<"."<<std::flush;

		for(int y=0; y<volumeSize[1]; y++){
			for(int z=0; z<volumeSize[2]; z++){

					filledVoxel = static_cast<unsigned char *>
							(filledVolume->GetScalarPointer(x, y, z));

					if (filledVoxel[0] == 0)
					{                 
						// create subvolume extent
						int subX1 = ((x - windowStep) < 0) ? 0 : x - windowStep;
						int subX2 = ((x + windowStep) >= volumeSize[0]) ? volumeSize[0]-1 : x + windowStep;
						int subY1 = ((y - windowStep) < 0) ? 0 : y - windowStep;
						int subY2 = ((y + windowStep) >= volumeSize[1]) ? volumeSize[1]-1 : y + windowStep;
						int subZ1 = ((z - windowStep) < 0) ? 0 : z - windowStep;
						int subZ2 = ((z + windowStep) >= volumeSize[2]) ? volumeSize[2]-1 : z + windowStep;

						for (int i = subX1; i <= subX2; i++){
							for (int j = subY1; j <= subY2; j++){
								for (int k = subZ1; k < subZ2; k++){
	                                
									if (x == i && y == j && z == k)
										continue;

									innerVoxel = static_cast<unsigned char *> (
											filledVolume->GetScalarPointer(i,j,k));

									if (innerVoxel[0] != 0)
									{
	                                    
										double distance = sqrt((x-i)^2+(y-j)^2+(z-k)^2);
	                                    
										//double w = exp(-0.5*pow(distance/maxDistance,2.0)); //Gaus
										double w = 1 - distance/maxDistance; //Lineal

										// get pointer to the current volume voxel 
										unsigned char *imageVoxel = static_cast<unsigned char *> (
												volumeData->GetScalarPointer(i,j,k));

										// get pointer to the current volume voxel 
										unsigned char * volumeVoxel = static_cast<unsigned char *> (
												volumeData->GetScalarPointer(x,y,z));

										// get pointer to the current accumator volume voxel 
										unsigned char * accDataVoxel = static_cast<unsigned char *> (
												AccDataVolume->GetScalarPointer(x,y,z);
															
										accDataVoxel[0]++;

										float temp = volumeVoxel[0] + (w*imageVoxel[0]-volumeVoxel[0])/accDataVoxel[0];
										volumeVoxel[0] = (unsigned char) temp;

									}
								}
							}
						}

	  
					}
				}
			}
    }

    
    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks * 10) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed filling volume: "<< double(diffms)<<" ms" <<std::endl;

	filledVolume->Delete();
	AccDataVolume->Delete();

}

void VolumeReconstructionPBM::holeFillingGrowingRegion()
{

	int maxKernelSize = 11;

    if(maxKernelSize%2 == 0)
        maxKernelSize -= 1;
    
    double maxDistance = calcMaxDistance();
    std::cout<<"Filling Empty Voxels with a Kernel of "<<maxKernelSize<<"x"<<maxKernelSize<<std::endl;
    
    clock_t begin = clock();

    // create volume pointers
    unsigned char * filledVoxel;
    unsigned char * innerVoxel;

    for(int x=0; x<volumeSize[0]; x++){
        
	std::cout<<"."<<std::flush;

	for(int y=0; y<volumeSize[1]; y++){
		for(int z=0; z<volumeSize[2]; z++){

                filledVoxel = static_cast<unsigned char *>
                        (filledVolume->GetScalarPointer(x, y, z));

                if (filledVoxel[0] == 0)
                {
                    // initial window size
                    int kernelSize = 1;
                    int windowStep = 0;
                    int idx = 0;

                    while ((idx == 0) && (kernelSize <= maxKernelSize))
                    {
						windowStep++;
                        kernelSize += 2;
                        
                        // create subvolume extent
                        int subX1 = ((x - windowStep) < 0) ? 0 : x - windowStep;
                        int subX2 = ((x + windowStep) >= volumeSize[0]) ? volumeSize[0]-1 : x + windowStep;
                        int subY1 = ((y - windowStep) < 0) ? 0 : y - windowStep;
                        int subY2 = ((y + windowStep) >= volumeSize[1]) ? volumeSize[1]-1 : y + windowStep;
                        int subZ1 = ((z - windowStep) < 0) ? 0 : z - windowStep;
                        int subZ2 = ((z + windowStep) >= volumeSize[2]) ? volumeSize[2]-1 : z + windowStep;

                        for (int i = subX1; i <= subX2; i++){
                            for (int j = subY1; j <= subY2; j++){
                                for (int k = subZ1; k < subZ2; k++){
                                    
                                    if (x == i && y == j && z == k)
                                        continue;

                                    innerVoxel = static_cast<unsigned char *> (
                                            filledVolume->GetScalarPointer(i,j,k));

                                    if (innerVoxel[0] != 0)
                                    {                         
                                        double distance = sqrt((x-i)^2+(y-j)^2+(z-k)^2);
	                                    
										//double w = exp(-0.5*pow(distance/maxDistance,2.0)); //Gaus
										double w = 1 - distance/maxDistance; //Lineal

										// get pointer to the current volume voxel 
										unsigned char *imageVoxel = static_cast<unsigned char *> (
												volumeData->GetScalarPointer(i,j,k));

										// get pointer to the current volume voxel 
										unsigned char * volumeVoxel = static_cast<unsigned char *> (
												volumeData->GetScalarPointer(x,y,z));

										// get pointer to the current accumator volume voxel 
										unsigned char * accDataVoxel = static_cast<unsigned char *> (
												AccDataVolume->GetScalarPointer(x,y,z);
															
										accDataVoxel[0]++;

										float temp = volumeVoxel[0] + (w*imageVoxel[0]-volumeVoxel[0])/accDataVoxel[0];
										volumeVoxel[0] = (unsigned char) temp;

                                        idx++;
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }
    }
    
    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks * 10) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed filling volume: "<< double(diffms)<<" ms" <<std::endl;

	filledVolume->Delete();
	AccDataVolume->Delete();
}

void VolumeReconstructionPBM::setScale(vnl_vector<double> scale)
{
    this->scale = scale;
}

void VolumeReconstructionPBM::setVolumeImageStack(
        std::vector< vtkSmartPointer<vtkImageData> > volumeImageStack)
{
    this->volumeImageStack = volumeImageStack;
}

void VolumeReconstructionPBM::setVolumeSize(vnl_vector<double> volumeSize)
{
    this->volumeSize = volumeSize;
}

void VolumeReconstructionPBM::setVolumeOrigin(vnl_vector<double> volumeOrigin)
{
    this->volumeOrigin = volumeOrigin;
}

void VolumeReconstructionPBM::setResolution(int resolution)
{
    this->resolution = resolution;
}

double VolumeReconstructionPBM::calcMaxDistance()
{
    double x = volumeSize[0]*scale[0]*resolution; 
    double y = volumeSize[1]*scale[1]*resolution; 
    double z = volumeSize[2]*scale[1]*resolution; 
        
    double maxDistance = sqrt(x*x + y*y + z*z);
	std::cout<<std::endl<<"Maximum distance in the volume: "<<maxDistance<<std::endl;

	return maxDistance;
}

void VolumeReconstructionPBM::setTransformStack(std::vector< vnl_matrix<double> > transformStack)
{
    this->transformStack = transformStack;
}
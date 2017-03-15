#include "VolumeReconstructionPBM.h"

#include <time.h>
#include <vtkMath.h>
#include <vtkImageMathematics.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkBMPWriter.h>
#include <vtkImageShiftScale.h>
#include <vtkImageLuminance.h>

#include <itkGradientMagnitudeImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <itkImageDuplicator.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>


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
    AccDataVolume->SetScalarTypeToFloat();
    AccDataVolume->SetNumberOfScalarComponents(1);
    AccDataVolume->SetOrigin(volumeOrigin[0],volumeOrigin[1],volumeOrigin[2]);
    AccDataVolume->SetDimensions(volumeSize[0],volumeSize[1],volumeSize[2]);
    AccDataVolume->SetSpacing(scale[0]*resolution,scale[1]*resolution,scale[1]*resolution);
    AccDataVolume->AllocateScalars();
    
    unsigned char * volumeVoxel;
    unsigned char * filledVoxel;
	float * accVoxel;
    
    for(int i=0; i<volumeSize[0]; i++){
		for(int j=0; j<volumeSize[1]; j++){
			for(int k=0; k<volumeSize[2]; k++){
                    
                    // get pointer to the current volume voxel   
                    volumeVoxel = static_cast<unsigned char *> (
                            volumeData->GetScalarPointer(i,j,k));
                    filledVoxel = static_cast<unsigned char *> (
                            filledVolume->GetScalarPointer(i,j,k));
					// get pointer to the current volume voxel   
                    accVoxel = static_cast<float *> (
                            AccDataVolume->GetScalarPointer(i,j,k));                                   
                    
                    volumeVoxel[0] = 0;
                    filledVoxel[0] = 0;
					accVoxel[0] = 0;
                    
                    
                }
        }
    }

    /////////////////////////////////
	this->binFillingGauss();
	this->holeFillingGrowingRegion();
  
    return volumeData;

}

void VolumeReconstructionPBM::binFillingNN()
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
                
				//voxel[0] = vtkMath::Floor((pixelCoordsXStack.at(i)[x][y] - volumeOrigin[0])/(scale[0]*resolution));
                //voxel[1] = vtkMath::Floor((pixelCoordsYStack.at(i)[x][y] - volumeOrigin[1])/(scale[1]*resolution));
                //voxel[2] = vtkMath::Floor((pixelCoordsZStack.at(i)[x][y] - volumeOrigin[2])/(scale[1]*resolution));                
                
                //if (voxel[0] < volumeSize[0] && voxel[0] > 0 && 
                //        voxel[1] < volumeSize[1] && voxel[1] > 0 &&
                //        voxel[2] < volumeSize[2] && voxel[2] > 0)
                //{                                                    
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
                    {
                        volumeVoxel[0] = imagePixel[0];
                        accDataVoxel[0] = 1;
                        fillVoxel[0] = 1;
                    }
                    else
                    {
                        float temp = ((volumeVoxel[0] * accDataVoxel[0]) + imagePixel[0]) /
                                (accDataVoxel[0] + 1);
                        volumeVoxel[0] = (unsigned char) temp;
                        accDataVoxel[0]++;
                    }      
                
                //}
          }
        }                
    
     }

    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks * 10) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed reconstructing volume: "<< double(diffms)<<" ms" <<std::endl;

}

void VolumeReconstructionPBM::binFillingGauss()
{

	float sigma = 1;
	float hSize = sigma*3;
	int wSize = 2*(vtkMath::Round(((2*hSize)+1)/2))-1;
	int wCenter = vtkMath::Floor(wSize/2); 
	std::vector<std::vector<std::vector<double> > > gaussKernel;
	
	gaussKernel.resize(wSize);
	for (int i = 0; i < wSize; ++i) {
		gaussKernel[i].resize(wSize);
		for (int j = 0; j < wSize; ++j)
			gaussKernel[i][j].resize(wSize);
    }

	for(int i=0;i<wSize;i++){
		for(int j=0;j<wSize;j++){
			for(int k=0;k<wSize;k++){

				float radius = pow(double(i-wCenter),2) + pow(double(j-wCenter),2) + pow(double(k-wCenter),2);
				gaussKernel[i][j][k] = exp(-radius/(2*pow(sigma,2)));


			}
		}
	}

	std::cout<<"Calculating voxel values with a gaussian kernel with sigma = "<<sigma<<" and a window size of "
		<<wSize<<"x"<<wSize<<"x"<<wSize<<std::flush;
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

				// create subvolume extent
				int subX1 = ((voxel[0] - wCenter) < 0) ? 0 : voxel[0] - wCenter;
				int subX2 = ((voxel[0] + wCenter) >= volumeSize[0]) ? volumeSize[0]-1 : voxel[0] + wCenter;
				int subY1 = ((voxel[1] - wCenter) < 0) ? 0 : voxel[1] - wCenter;
				int subY2 = ((voxel[1] + wCenter) >= volumeSize[1]) ? volumeSize[1]-1 : voxel[1] + wCenter;
				int subZ1 = ((voxel[2] - wCenter) < 0) ? 0 : voxel[2] - wCenter;
				int subZ2 = ((voxel[2] + wCenter) >= volumeSize[2]) ? volumeSize[2]-1 : voxel[2] + wCenter;

				unsigned char * volumeVoxel;

				for (int i = subX1; i <= subX2; i++){
					for (int j = subY1; j <= subY2; j++){
						for (int k = subZ1; k < subZ2; k++){
                

							// get pointer to the current volume voxel 
							volumeVoxel = static_cast<unsigned char *> (
									volumeData->GetScalarPointer(i,j,k));

							// get pointer to the current accumator volume voxel 
							float * accDataVoxel = static_cast<float *> (
									AccDataVolume->GetScalarPointer(i,j,k));


							// get pointer to the current fill volume voxel 
							unsigned char * fillVoxel = static_cast<unsigned char *> (
									filledVolume->GetScalarPointer(i,j,k));

							double gaussVal = gaussKernel[i-subX1][j-subY1][k-subZ1];

							// set voxel value with the corresponding pixel value
							if (fillVoxel[0] == 0)
								fillVoxel[0] = 1;

							//DAI
							float sum = volumeVoxel[0]*accDataVoxel[0] + imagePixel[0]*gaussVal;
							accDataVoxel[0] = accDataVoxel[0] + gaussVal;
							volumeVoxel[0] = sum/accDataVoxel[0];
							
							//MAX
							/*float temp = imagePixel[0]*gaussVal;
							if(volumeVoxel[0]<temp)
								volumeVoxel[0] = temp;*/

						}
					}
				}

          }
        }                
    
     }

    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed in bin-filling: "<< double(diffms)<<" ms" <<std::endl;
}

void VolumeReconstructionPBM::binFillingAdaptiveGauss()
{

	float maxVariance = 1.5;
	float step = 0.25;
	
	computeImageGradient();
	computeGaussKernels(maxVariance,step);

	std::vector<std::vector<std::vector<double> > > gaussKernel;

	std::cout<<"Calculating voxel values with a gaussian kernel with a max variance of "<<maxVariance<<std::endl;
    clock_t begin = clock();

	vnl_vector<double> point;
	point.set_size(4);
	point[0]=0;
	point[1]=0;
	point[2]=0;
	point[3]=1;

	std::vector<int> kernelCount;

    for (int i = 0; i < volumeImageStack.size(); i++)
	//for (int i = 357; i < 358; i++)
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

				unsigned char * gradientValue = static_cast<unsigned char *> (
                        gradientImageStack.at(i)->GetScalarPointer(x, y, 0));

				double temp = (double(gradientValue[0])/255)*(gaussKernelsStack.size() - 1);
				int kernel = vtkMath::Round(temp);
				gaussKernel = gaussKernelsStack.at(kernel);

				kernelCount.push_back(kernel);

				int wSize = gaussKernel.size();
				int wCenter = vtkMath::Floor(wSize/2); 

				// create subvolume extent
				int subX1 = ((voxel[0] - wCenter) < 0) ? voxel[0] : voxel[0] - wCenter;
				int subX2 = ((voxel[0] + wCenter) >= volumeSize[0]) ? volumeSize[0]-1 : voxel[0] + wCenter;
				int subY1 = ((voxel[1] - wCenter) < 0) ? 0 : voxel[1] - wCenter;
				int subY2 = ((voxel[1] + wCenter) >= volumeSize[1]) ? volumeSize[1]-1 : voxel[1] + wCenter;
				int subZ1 = ((voxel[2] - wCenter) < 0) ? 0 : voxel[2] - wCenter;
				int subZ2 = ((voxel[2] + wCenter) >= volumeSize[2]) ? volumeSize[2]-1 : voxel[2] + wCenter;

				unsigned char * volumeVoxel;

				for (int i = subX1; i <= subX2; i++){
					for (int j = subY1; j <= subY2; j++){
						for (int k = subZ1; k < subZ2; k++){
                

							// get pointer to the current volume voxel 
							volumeVoxel = static_cast<unsigned char *> (
									volumeData->GetScalarPointer(i,j,k));

							// get pointer to the current accumator volume voxel 
							float * accDataVoxel = static_cast<float *> (
									AccDataVolume->GetScalarPointer(i,j,k));


							// get pointer to the current fill volume voxel 
							unsigned char * fillVoxel = static_cast<unsigned char *> (
									filledVolume->GetScalarPointer(i,j,k));

							double gaussVal = gaussKernel[i-subX1][j-subY1][k-subZ1];

							// set voxel value with the corresponding pixel value
							if (fillVoxel[0] == 0)
								fillVoxel[0] = 1;

							//DAI
							float sum = volumeVoxel[0]*accDataVoxel[0] + imagePixel[0]*gaussVal;
							accDataVoxel[0] = accDataVoxel[0] + gaussVal;
							volumeVoxel[0] = sum/accDataVoxel[0];

						}
					}
				}

          }
        }                
    
     }


	for(int b=0;b<gaussKernelsStack.size();b++){
		int mycount = std::count(kernelCount.begin(), kernelCount.end(), b);
		std::cout<<b<<" count: "<<mycount<<std::endl;
	}

	std::cout<<"Number of pixels: "<<kernelCount.size()<<std::endl;

    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed in bin filling: "<< double(diffms)<<"s" <<std::endl;
}

void VolumeReconstructionPBM::holeFillingFixedRegion()
{

	int cont = 0;

	int kernelSize = 9;
	int windowStep = vtkMath::Floor(kernelSize/2);
    
    double maxDistance = calcMaxDistance();
    std::cout<<"Filling Empty Voxels with a Kernel of "<<kernelSize<<std::endl;
    
    clock_t begin = clock();

    // create an interpolation buffer
    vtkSmartPointer<vtkImageData> InterpolationVolume = vtkSmartPointer<vtkImageData>::New();
    InterpolationVolume->SetScalarTypeToUnsignedChar();
    InterpolationVolume->SetNumberOfScalarComponents(1);
    InterpolationVolume->SetOrigin(volumeOrigin[0],volumeOrigin[1],volumeOrigin[2]);
    InterpolationVolume->SetDimensions(volumeSize[0],volumeSize[1],volumeSize[2]);
    InterpolationVolume->SetSpacing(scale[0]*resolution,scale[1]*resolution,scale[1]*resolution);
    InterpolationVolume->AllocateScalars();   
    
    unsigned char * interpolationVoxel;
    
    for(int i=0; i<volumeSize[0]; i++){
	for(int j=0; j<volumeSize[1]; j++){
		for(int k=0; k<volumeSize[2]; k++){
                    
                    // get pointer to the current volume voxel   
                    interpolationVoxel = static_cast<unsigned char *> (
                            InterpolationVolume->GetScalarPointer(i,j,k));                   
                    
                    interpolationVoxel[0] = 0;                                    
                }
        }
    }


    // create volume pointers
    unsigned char * voxel;
    unsigned char * filledVoxel;
    unsigned char * innerVoxel;
    unsigned char * interpVoxel;
    
    vnl_vector<double> centralVoxel;
    centralVoxel.set_size(3);
    
    vnl_vector<double> foundVoxel;
    foundVoxel.set_size(3);

    for(int x=0; x<volumeSize[0]; x++){
        
	std::cout<<"."<<std::flush;

	for(int y=0; y<volumeSize[1]; y++){
		for(int z=0; z<volumeSize[2]; z++){

                filledVoxel = static_cast<unsigned char *>
                        (filledVolume->GetScalarPointer(x, y, z));

                if (filledVoxel[0] == 0)
                {
                    // center window point
                    centralVoxel[0] = x;
                    centralVoxel[1] = y;
                    centralVoxel[2] = z;

                    float acc = 0;
                    int idx = 0;

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
                                            volumeData->GetScalarPointer(i, j, k));

                                    if (innerVoxel[0] != 0)
                                    {
                                        foundVoxel[0] = i;
                                        foundVoxel[1] = j;
                                        foundVoxel[2] = k;
                                        
                                        double distance = vnl_vector_ssd(centralVoxel,foundVoxel);
                                        
                                        double w = exp(-0.5*pow(distance/maxDistance,2.0)); //Gaus
                                        //double w = 1 - distance/maxDistance; //Lineal

                                        acc += w*innerVoxel[0];

                                        idx++;
                                    }
                                }
                            }
                        }

                        if (idx != 0)
                        {
                            acc /= idx;

                            interpVoxel = static_cast<unsigned char*>
                                    (InterpolationVolume->GetScalarPointer(x, y, z));
                            interpVoxel[0] = acc;
                        }
                      
                }
            }
        }
    }


    for(int x=0; x<volumeSize[0]; x++){
	for(int y=0; y<volumeSize[1]; y++){
		for(int z=0; z<volumeSize[2]; z++){
                    
                    
                interpVoxel = static_cast<unsigned char*>
                        (InterpolationVolume->GetScalarPointer(x, y, z));
                
                if (interpVoxel[0] != 0)
                {
                    voxel = static_cast<unsigned char *>
                            (volumeData->GetScalarPointer(x, y, z));



                    voxel[0] = interpVoxel[0];
                }
            }
        }
    }
    
    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks * 10) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed filling volume: "<< double(diffms)<<" ms" <<std::endl;

}

void VolumeReconstructionPBM::holeFillingGrowingRegion()
{

	int cont = 0;
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
                                        double distance = sqrt(pow(double(x-i),2)+pow(double(y-j),2)+pow(double(z-k),2));
	                                    
										//double w = exp(-0.5*pow(distance/maxDistance,2.0)); //Gaus
										double w = 1 - distance/maxDistance; //Lineal

										// get pointer to the current volume voxel 
										unsigned char *imageVoxel = static_cast<unsigned char *> (
												volumeData->GetScalarPointer(i,j,k));

										// get pointer to the current volume voxel 
										unsigned char * volumeVoxel = static_cast<unsigned char *> (
												volumeData->GetScalarPointer(x,y,z));

										// get pointer to the current accumator volume voxel 
										float * accDataVoxel = static_cast<float *> (
												AccDataVolume->GetScalarPointer(x,y,z));
															
										accDataVoxel[0]++;

										float temp = volumeVoxel[0] + (w*imageVoxel[0]-volumeVoxel[0])/accDataVoxel[0];
										volumeVoxel[0] = (unsigned char) temp;

                                        idx++;
                                    }
                                }
                            }
                        }

                    }
                
					cont++;
				
				}
            }
        }
    }
    
    clock_t end = clock();
    double diffticks = end - begin;
    double diffms = (diffticks) / CLOCKS_PER_SEC;
    std::cout<<std::endl<<"Time elapsed in hole-filling: "<< double(diffms)<<" s" <<std::endl;
	std::cout<<cont<<" voxels filled in the hole-filling step"<<std::endl;
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

void VolumeReconstructionPBM::computeGaussKernels(float maxVariance, float step)
{

	int nKernels = maxVariance/step;
	float sigma = maxVariance;

	std::cout<<"Creating "<<nKernels<<" gaussian kernels with a variance step of "<<step<<std::endl;

	for(int k=0;k<nKernels;k++){

		float hSize = sigma*3;
		int wSize = 2*(vtkMath::Round(((2*hSize)+1)/2))-1;
		int wCenter = vtkMath::Floor(wSize/2); 
		std::vector<std::vector<std::vector<double> > > gaussKernel;

		gaussKernel.resize(wSize);
		for (int i = 0; i < wSize; ++i) {
			gaussKernel[i].resize(wSize);
			for (int j = 0; j < wSize; ++j)
				gaussKernel[i][j].resize(wSize);
		}

		for(int i=0;i<wSize;i++){
			for(int j=0;j<wSize;j++){
				for(int k=0;k<wSize;k++){

					float radius = pow(double(i-wCenter),2) + pow(double(j-wCenter),2) + pow(double(k-wCenter),2);
					gaussKernel[i][j][k] = exp(-radius/(2*pow(sigma,2)));


				}
			}
		}

		gaussKernelsStack.push_back(gaussKernel);
		sigma -= step;

	}
	
}

void VolumeReconstructionPBM::computeImageGradient()
{

	std::cout<<"Computing image gradient for 2D images"<<std::endl;
	for(int i=0;i<volumeImageStack.size();i++){

		typedef itk::GradientMagnitudeImageFilter<FloatImageType,FloatImageType> GradientFilterType;
		GradientFilterType::Pointer gradientFilter = GradientFilterType::New();
		gradientFilter->SetInput(this->convertToITKImage(volumeImageStack.at(i)));
		gradientFilter->Update();

		typedef itk::RescaleIntensityImageFilter<FloatImageType,FloatImageType> RescaleFilterType;
    
		RescaleFilterType::Pointer normalizeFilter = RescaleFilterType::New();
		normalizeFilter->SetOutputMaximum(255);
		normalizeFilter->SetOutputMinimum(0);
		normalizeFilter->SetInput(gradientFilter->GetOutput());
		normalizeFilter->Update(); 

		gradientImageStack.push_back(this->convertToVTKImage(normalizeFilter->GetOutput()));


		/*vtkSmartPointer<vtkImageGradientMagnitude> imageGradientFilter = vtkSmartPointer<vtkImageGradientMagnitude>::New();
		imageGradientFilter->SetInput(volumeImageStack.at(i));
		imageGradientFilter->Update();

		vtkSmartPointer<vtkImageData> gradientImage = imageGradientFilter->GetOutput();

		float oldRange = gradientImage->GetScalarRange()[1] - gradientImage->GetScalarRange()[0];
		float newRange = 255;
		
		vtkSmartPointer<vtkImageShiftScale> shiftScaleFilter = vtkSmartPointer<vtkImageShiftScale>::New();
		shiftScaleFilter->SetOutputScalarTypeToUnsignedChar();
		shiftScaleFilter->SetInput(gradientImage);
		shiftScaleFilter->SetShift(-1.0f * gradientImage->GetScalarRange()[0]);
		shiftScaleFilter->SetScale(newRange/oldRange);
		shiftScaleFilter->Update();*/

		//gradientImageStack.push_back(shiftScaleFilter->GetOutput());

	}

	/*vtkSmartPointer<vtkBMPWriter> writer = vtkSmartPointer<vtkBMPWriter>::New();
	
	QString saveDirectory = QString("C:/Users/Fubu/Desktop/Gradiente");

	QString filename;
	std::string str;
	const char * saveFile;

	for(int i=0; i < gradientImageStack.size(); i++){			
			char imageNumber[4];
			sprintf(imageNumber,"%d",i);
			
			filename = saveDirectory;

			if(i<10)
				filename.append("/IMG000");
			else if(i<100)
				filename.append("/IMG00");
			else if(i<1000)
				filename.append("/IMG0");
			else
				filename.append("/IMG");

			filename.append(imageNumber);
			filename.append(".bmp");

			str = std::string(filename.toAscii().data());
			saveFile = str.c_str();

			writer->SetFileName(saveFile);
			writer->SetInput(gradientImageStack.at(i));
			writer->Write();
	
	}*/

}

typedef itk::Image<float, 2> FloatImageType;
FloatImageType::Pointer VolumeReconstructionPBM::convertToITKImage(vtkSmartPointer<vtkImageData> vtkImage)
{
    
    int numberOfScalars = vtkImage->GetNumberOfScalarComponents();
    
    typedef itk::VTKImageToImageFilter<ImageType> ITKConverterType;
    ITKConverterType::Pointer itkConverter = ITKConverterType::New();
    
    if(numberOfScalars==3){
        vtkSmartPointer<vtkImageLuminance> imageLuminance = vtkSmartPointer<vtkImageLuminance>::New();
        imageLuminance->SetInput(vtkImage);
        imageLuminance->Update();
        itkConverter->SetInput(imageLuminance->GetOutput());
    }else{
        itkConverter->SetInput(vtkImage);
    }

	itkConverter->Update();
    
    typedef itk::ImageDuplicator<ImageType> DuplicatorType;
    DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(itkConverter->GetOutput());
    duplicator->Update();
        
    typedef itk::CastImageFilter<ImageType,FloatImageType> CastImageType;
    CastImageType::Pointer castImage = CastImageType::New();
    castImage->SetInput(duplicator->GetOutput());
    castImage->Update();
    
    return castImage->GetOutput();

}

vtkSmartPointer<vtkImageData> VolumeReconstructionPBM::convertToVTKImage(FloatImageType::Pointer itkImage)
{  
	CastFilterType::Pointer castProbabilityFilter = CastFilterType::New();
	castProbabilityFilter->SetInput(itkImage);
	castProbabilityFilter->Update(); 
    
	typedef itk::ImageToVTKImageFilter<ImageType> VTKConverterType;
	VTKConverterType::Pointer vtkConverter = VTKConverterType::New();
	vtkConverter->SetInput(castProbabilityFilter->GetOutput());
	vtkConverter->Update();
    
	vtkSmartPointer<vtkImageData> tempImage = vtkSmartPointer<vtkImageData>::New();
	tempImage->DeepCopy(vtkConverter->GetOutput());
    
	return tempImage;
}
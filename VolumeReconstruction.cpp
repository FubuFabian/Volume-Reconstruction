#include "VolumeReconstruction.h"

#include <vtkMath.h>
#include <vtkMetaImageWriter.h>
#include <vnl/vnl_inverse.h>
#include <exception>

vtkSmartPointer<vtkImageData> VolumeReconstruction::generateVolume()
{
	std::cout<<"Generating Volume Data"<<std::endl;

	vtkSmartPointer<vtkImageData> volumeData = vtkSmartPointer<vtkImageData>::New();
	volumeData->SetNumberOfScalarComponents(1);
	volumeData->SetScalarType(VTK_UNSIGNED_CHAR);
	volumeData->SetOrigin(0,0,0);
	volumeData->SetDimensions(volumeSize[0],volumeSize[1],volumeSize[2]);
	volumeData->SetSpacing(scale[0],scale[0],scale[0]);
	volumeData->AllocateScalars();

	calcImagePlane();
	maxDistance = calcMaxDistance();

	std::cout<<"Calculating voxel values";

	for(int i=215; i<volumeSize[0]; i++){
		
		std::cout<<".";

		for(int j=82; j<volumeSize[1]; j++){
			for(int k=230; k<volumeSize[2]; k++){

				double voxel[3];
				voxel[0] = i*scale[0] + volumeOrigin[0];
				voxel[1] = j*scale[1] + volumeOrigin[1];
				voxel[2] = k*scale[1] + volumeOrigin[2];

				vnl_vector<double>	distance;
				distance.set_size(3);
				distance.fill(maxDistance);

				vnl_vector<double>	distancePlane;
				distancePlane.set_size(3);
				

				for(int plane=0; plane<volumeImageStack.size(); plane++){				
					
					double d = imagePlaneStack.at(plane)->DistanceToPlane(voxel);
					
					if(d <= distance[0]){
						
						distance.put(2,distance[1]);
						distance.put(1,distance[0]);
						distance.put(0,d);

						distancePlane.put(2,distancePlane[1]);
						distancePlane.put(1,distancePlane[0]);
						distancePlane.put(0,plane);
					
					}else if(d <= distance[1]){

						distance.put(2,distance[1]);
						distance.put(1, d);

						distancePlane.put(2,distancePlane[1]);
						distancePlane.put(1,plane);

					}else if(d <= distance[2]){

						distance.put(2,d);

						distancePlane.put(2,plane);

					}
				}

				std::vector< vnl_vector<double> > crossPoints;
				crossPoints.reserve(3);

				double crossPoint[3];
				vnl_vector<double> crossPointVector;
				crossPointVector.set_size(3);

				imagePlaneStack.at(distancePlane[0])->ProjectPoint(voxel,crossPoint);
				crossPointVector.put(0,crossPoint[0]);
				crossPointVector.put(1,crossPoint[1]);
				crossPointVector.put(2,crossPoint[2]);
				
				crossPoints.push_back(crossPointVector);

				imagePlaneStack.at(distancePlane[1])->ProjectPoint(voxel,crossPoint);
				crossPointVector.put(0,crossPoint[0]);
				crossPointVector.put(1,crossPoint[1]);
				crossPointVector.put(2,crossPoint[2]);
				
				crossPoints.push_back(crossPointVector);

				imagePlaneStack.at(distancePlane[2])->ProjectPoint(voxel,crossPoint);
				crossPointVector.put(0,crossPoint[0]);
				crossPointVector.put(1,crossPoint[1]);
				crossPointVector.put(2,crossPoint[2]);
				
				crossPoints.push_back(crossPointVector);

				double voxelValue = calcVoxelValue(crossPoints, distancePlane, distance);

				unsigned char* pixel = static_cast<unsigned char*>(volumeData->GetScalarPointer(i,j,k));

				pixel[0] = voxelValue;
			}
		}
	}

	 
	vtkSmartPointer<vtkMetaImageWriter> writer = vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetFileName("C:/Users/Administrador/Documents/Volumenes/test_03.mhd");
	writer->SetRAWFileName("C:/Users/Administrador/Documents/Volumenes/test_03.raw");
	writer->SetInputConnection(volumeData->GetProducerPort());
	try{
	writer->Write();
	}catch( exception& e ){
		std::cout<<e.what()<<std::endl;
	}

	return volumeData;

}

double VolumeReconstruction::calcVoxelValue(std::vector< vnl_vector<double>> crossPoints, 
											vnl_vector<double> distancePlane, vnl_vector<double> distance)
{
	double voxelValue = 0;
	int prom = 0;
	
	for(int i=0; i<crossPoints.size(); i++){

//////////////////////
		double plano = distancePlane[i];
		double distancia = distance[i];
/////////////////////

		vnl_matrix<double> inverseTransform = vnl_inverse(transformStack.at(distancePlane[i]));
	
		vnl_vector<double> pointCoords;
		pointCoords.set_size(4);
		pointCoords.put(0,crossPoints.at(i)[0]);
		pointCoords.put(1,crossPoints.at(i)[1]);
		pointCoords.put(2,crossPoints.at(i)[2]);
		pointCoords.put(3,1);

////////////////////
		double punto1 = crossPoints.at(i)[0];
		double punto2 = crossPoints.at(i)[1];
		double punto3 = crossPoints.at(i)[2];
////////////////////

		vnl_vector<double> imgCoord = inverseTransform*pointCoords; 

		int x = imgCoord[0]/scale[0];
		int y = imgCoord[1]/scale[1];

		double pixelValue= 0;

		int * imgSize = volumeImageStack.at(distancePlane[i])->GetDimensions();

		if(x>1 && y>1){
			if(x<imgSize[0]-1 && y<imgSize[1]-1){

				prom++;
				
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x-1,y-1,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x,y-1,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x+1,y-1,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x-1,y,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x,y,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x+1,y,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x-1,y+1,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x,y+1,0,0);
				pixelValue += volumeImageStack.at(distancePlane[i])->GetScalarComponentAsDouble(x+1,y+1,0,0);

				pixelValue /= 9;

				double w = 1 - distance[distancePlane[i]]/maxDistance;

				pixelValue *= w;
				voxelValue += pixelValue;

			}
		}

	}

	voxelValue /= prom;

	return voxelValue;

}


double VolumeReconstruction::calcMaxDistance()
{
	double maxDistance = sqrt(volumeSize[0]*volumeSize[0] + volumeSize[1]*volumeSize[1] + volumeSize[2]*volumeSize[2]);
	std::cout<<std::endl<<"Maximum distance in the volume: "<<maxDistance<<std::endl;

	return maxDistance;
}

void VolumeReconstruction::calcImagePlane()
{
	std::cout<<std::endl;
	std::cout<<"Calculating images planes"<<std::endl<<std::endl;

	for(int i=0; i<volumeImageStack.size(); i++){

		vnl_vector<double> xBounds = imageBoundsXStack.at(i);
		vnl_vector<double> yBounds = imageBoundsYStack.at(i);
		vnl_vector<double> zBounds = imageBoundsZStack.at(i);

		vnl_vector<double> AB;
		AB.set_size(3);

		AB[0] = xBounds[1] - xBounds[0];
		AB[1] = yBounds[1] - yBounds[0];
		AB[2] = zBounds[1] - zBounds[0];

		vnl_vector<double> AC;
		AC.set_size(3);

		AC[0] = xBounds[2] - xBounds[0];
		AC[1] = yBounds[2] - yBounds[0];
		AC[2] = zBounds[2] - zBounds[0];

		double a;
		double b;
		double c;

		a = (AB[1]*AC[2]) - (AC[1]*AB[2]);
		b = (AC[0]*AB[2]) - (AB[0]*AC[2]);
		c = (AB[0]*AC[1]) - (AC[0]*AB[1]);

		double planeMagnitud = sqrt(a*a + b*b + c*c);
		
		a = a/planeMagnitud;
		b = b/planeMagnitud;
		c = c/planeMagnitud;

		vtkSmartPointer<vtkPlane> imagePlane = vtkSmartPointer<vtkPlane>::New();

		imagePlane->SetOrigin(xBounds[0],yBounds[0],zBounds[0]);
		imagePlane->SetNormal(a,b,c);

		imagePlaneStack.push_back(imagePlane);
	}
}

void VolumeReconstruction::setImageBoundsStack(std::vector< vnl_vector<double> > imageBoundsXStack
                                               , std::vector< vnl_vector<double> > imageBoundsYStack
                                               , std::vector< vnl_vector<double> > imageBoundsZStack)
{
    this->imageBoundsXStack = imageBoundsXStack;
    this->imageBoundsYStack = imageBoundsYStack;
    this->imageBoundsZStack = imageBoundsZStack;
}

void VolumeReconstruction::setScale(vnl_vector<double> scale)
{
    this->scale = scale;
}

void VolumeReconstruction::setTransformStack(std::vector< vnl_matrix<double> > transformStack)
{
    this->transformStack = transformStack;
}

void VolumeReconstruction::setVolumeImageStack(
        std::vector< vtkSmartPointer<vtkImageData> > volumeImageStack)
{
    this->volumeImageStack = volumeImageStack;
}

void VolumeReconstruction::setVolumeSize(vnl_vector<double> volumeSize)
{
    this->volumeSize = volumeSize;
}

void VolumeReconstruction::setVolumeOrigin(vnl_vector<double> volumeOrigin)
{
    this->volumeOrigin = volumeOrigin;
}

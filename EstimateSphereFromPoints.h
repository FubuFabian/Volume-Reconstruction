#ifndef ESTIMATESPHEREFROMPOINTS_H
#define ESTIMATESPHEREFROMPOINTS_H

#endif // ESTIMATESPHEREFROMPOINTS_H

#include <vtkSmartPointer.h>
#include <vtkPoints.h>

class EstimateSphereFromPoints
{

public:

    static EstimateSphereFromPoints *New()
    {
        return new EstimateSphereFromPoints;
    }

    void setPoints(vtkSmartPointer< vtkPoints >);

private:

    vtkSmartPointer< vtkPoints > points;

};

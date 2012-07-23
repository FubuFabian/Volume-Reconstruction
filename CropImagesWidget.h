#ifndef CROPIMAGESWIDGET_H
#define CROPIMAGESWIDGET_H

#include "ui_CropImagesWidget.h"
#include "mainwindow.h"

#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

namespace Ui {
class CropImagesWidget;
}

class CropImagesWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit CropImagesWidget(QWidget *parent = 0);
    ~CropImagesWidget();

     /**
     * \brief Set this stack of vtkImageData 
     * \param[in] a std Vector of vtkImageData
     */
    void setImageStack(std::vector< vtkSmartPointer<vtkImageData> > imageStack);
    
    /**
     * \brief Set this vtkImageData 
     * \param[in] a smart Pointer of vtkImageData
     */
    void setImage(vtkSmartPointer<vtkImageData> image);
    
    
    /** 
	 * \brief Set the window to display the crop images 
	 */
    void setMainWindow(MainWindow* mainwindow);

private:
    Ui::CropImagesWidget *ui;

	 bool workWithStack;
    
    /** \brief an Array of vtkImageData to work */
    std::vector< vtkSmartPointer<vtkImageData> > imageStack;
	 
    /** the main window to call it */
    MainWindow* mainWindow;

	 /** \brief the vtkImageData to work */
    vtkSmartPointer<vtkImageData> image;

	vtkSmartPointer<vtkImageData> cropImage;

    std::vector< vtkSmartPointer<vtkImageData> > cropStack;
    
    
    /** Crop ultrasound image depnding of the depth type*/
    vtkSmartPointer<vtkImageData> cropProbeImage(vtkSmartPointer<vtkImageData> image, int depthType);

private slots:

    void crop();

    void save();

};

#endif // CROPIMAGESWIDGET_H

/********************************
 *
****************************/
#include "function.h"
#include <QMessageBox>

#include <assert.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
//Function::Function()
//{

//}
#include <QFile>

QImage* Function::CjwIplToQImg(const IplImage *cvImg)
{
    if(!cvImg)
    {
        return NULL;
    }
    IplImage* temp=NULL;
    temp=cvCloneImage(cvImg);
    cvCvtColor(cvImg,temp, CV_BGR2RGB);

    uchar* imgData = (uchar*)(temp->imageData);
    QImage *qImg=new QImage(imgData,temp->width,temp->height,QImage::Format_RGB888);
    return qImg;
    cvReleaseImage(&temp);
    delete imgData;
}


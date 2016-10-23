#ifndef QKMEAS
#define QKMEAS

#endif // QKMEAS

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv/cxcore.h>

void kmeans(IplImage* src,IplImage* dst);
void chuli(IplImage* img,IplImage* dst);
void max_liantong(IplImage* src,IplImage* dst);
IplImage* reverse(IplImage* img);

#ifndef FUNCTION_H
#define FUNCTION_H
#include <QImage>
#include <fcntl.h>
//#include <opencv2/core.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
//#include <linux/videodev2.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#define CLEAR(x) memset (&(x), 0, sizeof (x))

class Function
{
public:
//    Function();
    static QImage* CjwIplToQImg(const IplImage *cvImg);

private:

};

#endif // FUNCTION_H

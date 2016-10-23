#ifndef MYLABLE_H
#define MYLABLE_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>
#include <QWidget>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include "videodevice.h"

class MyLable : public QLabel
{
    Q_OBJECT
public:
    explicit MyLable(QWidget *parent = 0);
    //void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void closeEvent(QCloseEvent *event);

    void setPixmap(const uchar* buf);
            QImage *frame;
        bool video_or_pic;
        bool pic;
        bool mouseMove;
signals:
    void send_rgb_num(int red,int green,int blue);
    void send_s_l(double width, double hight, double s, double l);

protected:
    void clear_list();
    void watersged();
    void auto_show_pic();
    void auto_kmeans();
    void set_scaled(QVector<QPoint> list,int num);
    ~MyLable();

public slots:
        void set_filename();
       int set_scaled_button();
//        void display_error(QString err);
        void paintEvent(QPaintEvent *event);
        void show_picture();
        void auto_show_pic_btn();
//        void up_data();
private:
        QString set_pic_filename_path;
        QImage* img;
        QImage image;
        QList<int> listx;
        QList<int> listy;
        QVector<QPoint> list;
        QVector<QPoint> list1;
        QVector<QPoint> list2;
        QVector<QPoint> list3;
        int num_list;
        int red;
        int blue;
        int green;
        int x;
        int y;
        int num;

        IplImage *markes;
        IplImage *marker_mask;
        CvPoint prev_pt;

        CvMemStorage* storage;
        IplImage* img0;
        IplImage* draw_img;
        IplImage* wshed;
        IplImage* img_gray;
        IplImage* tongue_body;
        IplImage* tongue_body_1;
        IplImage* tongue_body_bw;
        CvRNG rng;
//        IplImage *image;
        IplImage *hsv;
        IplImage *hue;
        IplImage *saturation;
        IplImage *value;//图像空间

        QPainter *painter;
//        QImage *frame;
        QTimer *timer;
        int rs;
        uchar *pp;
        uchar * p;
        unsigned int len;
//        int convert_yuv_to_rgb_pixel(int y, int u, int v);
//        int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
        VideoDevice *vd;
        QPixmap pixmap;
};

#endif // MYLABLE_H

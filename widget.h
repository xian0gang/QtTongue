#ifndef WIDGET_H
#define WIDGET_H

#include <QDebug>
#include <QWidget>
#include "videodevice.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

signals:
    void send_filename(QString filename);
    void send_set_scale();
    void show_pic();
    void auto_show_pic();

public slots:
    void OpenFile();
    void close_pushbutton();
    void set_scale_pushbutton();
    void capture_pic();
    void up_date();
    void display_error(QString err);
    void set_s_l(double width, double hight, double s, double l);
    void auto_show_picture();
private:
    Ui::Widget *ui;
    QString filename;

    QPainter *painter;
//    QImage *frame;
    QTimer *timer;
    int rs;
    uchar *pp;
    uchar * p;
    unsigned int len;
    int convert_yuv_to_rgb_pixel(int y, int u, int v);
    int convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height);
    VideoDevice *vd;
};

#endif // WIDGET_H

#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <qmath.h>

#define PI 3.1415926

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //markes = cvCreateImage()
    /************************************/
    pp = (unsigned char *)malloc(320 * 240/*QWidget::width()*QWidget::height()*/* 3 * sizeof(char));

    ui->label->frame = new QImage(pp,320,240,QImage::Format_RGB888);
   // frame = new QPixmap(640,320);

    vd = new VideoDevice(tr("/dev/video1"));

    connect(vd, SIGNAL(display_error(QString)), this,SLOT(display_error(QString)));
    rs = vd->open_device();
     qDebug("open_device");
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("open /dev/dsp error"),QMessageBox::Yes);
        vd->close_device();
        qDebug("close_device");
    }

    rs = vd->init_device();
    qDebug("init_device");
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("init failed"),QMessageBox::Yes);
        vd->close_device();
    }

    rs = vd->start_capturing();
    qDebug("start_capturing");
    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("start capture failed"),QMessageBox::Yes);
        vd->close_device();
    }

    if(-1==rs)
    {
        QMessageBox::warning(this,tr("error"),tr("get frame failed"),QMessageBox::Yes);
        vd->stop_capturing();
    }

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(up_date()));
    timer->start(80);
    /*****************************************/

    //open file
   connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(OpenFile()));
   //send filename
   connect(this,SIGNAL(send_filename(QString)),ui->label,SLOT(set_filename()));

   //关闭
   connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(close_pushbutton()));

   connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(set_scale_pushbutton()));
   connect(this,SIGNAL(send_set_scale()),ui->label,SLOT(set_scaled_button()));

   //拍照
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(capture_pic()));
    connect(this,SIGNAL(show_pic()),ui->label,SLOT(show_picture()));

    //自动处理轮廓
    connect(ui->pushButton_5,SIGNAL(clicked(bool)),this,SLOT(auto_show_picture()));
    connect(this,SIGNAL(auto_show_pic()),ui->label,SLOT(auto_show_pic_btn()));

    connect(ui->label,SIGNAL(send_s_l(double,double,double,double)),this,SLOT(set_s_l(double,double,double,double)));

}

Widget::~Widget()
{
    delete ui;
}

//打开文件    图片
void Widget::OpenFile()
{
    emit send_filename(filename);
    timer->start(80);
}

//利用opencv获取轮廓
void Widget::set_scale_pushbutton()
{
    emit send_set_scale();
}

//关闭程序
void Widget::close_pushbutton()
{
    this->close();
}

//拍照
void Widget::capture_pic()
{
    //出发拍照信号
    emit show_pic();
    timer->stop();
}

void Widget::auto_show_picture()
{
    emit auto_show_pic();
}

void Widget::set_s_l(double width, double hight, double s, double t)
{
    ui->label_s->setText(QString("%1").arg(s));
    ui->label_l->setText(QString("%1").arg(t));

    /******圆度********/
    double R;
    R = (4*PI*t)/(qPow(t,2));
    ui->label_yuan->setText(QString("%1").arg(R));

    /******长宽比********/
    double I;
    I = fabs(width-hight)/hight;
    ui->label_w_h->setText(QString("%1").arg(I));

    /*******矩形度*******/
    double R1;
    R1 = s/(width*hight);
    ui->label_juxingdu->setText(QString("%1").arg(R1));

}

void Widget::up_date()
{
    rs = vd->get_frame((void **)&p,&len);
    convert_yuv_to_rgb_buffer(p,pp,320,240/*QWidget::width(),QWidget::height()*/);
//    frame->loadFromData((uchar *)pp,/*len*/640 * 480 * 3 * sizeof(char));
//    ui->label->frame->loadFromData((uchar *)pp,/*len*/640 * 480 * 3 * sizeof(char));
    ui->label->setPixmap(pp);
    vd->unget_frame();
}

void Widget::display_error(QString err)
{
    QMessageBox::warning(this,tr("error"), err,QMessageBox::Yes);
}

/***********************************
 * 函  数  名：convert_yuv_to_rgb_buffer(unsigned char *yuv,
 *                                                                  unsigned char *rgb,
 *                                                                  unsigned int width,
 *                                                                  unsigned int height)
 * 功        能：把采集的yuv视频数据转换为rgb用于保存图片和显示
 *
 * 参        数：unsigned char *yuv
 *                     unsigned char *rgb
 *                     unsigned int width,
 *                     unsigned int height
 * 返  回  值：0
 *
 * ***********************************/
int Widget::convert_yuv_to_rgb_buffer(unsigned char *yuv, unsigned char *rgb, unsigned int width, unsigned int height)
{
 unsigned int in, out = 0;
 unsigned int pixel_16;
 unsigned char pixel_24[3];
 unsigned int pixel32;
 int y0, u, y1, v;
 for(in = 0; in < width * height * 2; in += 4) {
  pixel_16 =
   yuv[in + 3] << 24 |
   yuv[in + 2] << 16 |
   yuv[in + 1] <<  8 |
   yuv[in + 0];
  y0 = (pixel_16 & 0x000000ff);
  u  = (pixel_16 & 0x0000ff00) >>  8;
  y1 = (pixel_16 & 0x00ff0000) >> 16;
  v  = (pixel_16 & 0xff000000) >> 24;
  pixel32 = convert_yuv_to_rgb_pixel(y0, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
  pixel32 = convert_yuv_to_rgb_pixel(y1, u, v);
  pixel_24[0] = (pixel32 & 0x000000ff);
  pixel_24[1] = (pixel32 & 0x0000ff00) >> 8;
  pixel_24[2] = (pixel32 & 0x00ff0000) >> 16;
  rgb[out++] = pixel_24[0];
  rgb[out++] = pixel_24[1];
  rgb[out++] = pixel_24[2];
 }
 return 0;
}

/***********************************
 * 函  数  名：convert_yuv_to_rgb_pixel(int y, int u, int v)
 * 功        能：
 *
 * 参        数：int y, int u, int v
 * 返  回  值：pixel32
 *
 * ***********************************/
int Widget::convert_yuv_to_rgb_pixel(int y, int u, int v)
{
 unsigned int pixel32 = 0;
 unsigned char *pixel = (unsigned char *)&pixel32;
 int r, g, b;
 r = y + (1.370705 * (v-128));
 g = y - (0.698001 * (v-128)) - (0.337633 * (u-128));
 b = y + (1.732446 * (u-128));
 if(r > 255) r = 255;
 if(g > 255) g = 255;
 if(b > 255) b = 255;
 if(r < 0) r = 0;
 if(g < 0) g = 0;
 if(b < 0) b = 0;
 pixel[0] = r * 220 / 256;
 pixel[1] = g * 220 / 256;
 pixel[2] = b * 220 / 256;
 return pixel32;
}

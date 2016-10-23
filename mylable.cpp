#include "mylable.h"
#include "function.h"
#include <stdio.h>
#include <QMessageBox>
#include <qmath.h>
#include "qkmeas.h"
#include <QDebug>

using namespace cv;
using namespace std;

//主
MyLable::MyLable(QWidget *parent) : QLabel(parent)
{
    num_list = 0;
    video_or_pic = true;
    pic = true;
    mouseMove = false;
    //初始化起始点，必须有
    prev_pt =cvPoint (-1,-1);

    storage = cvCreateMemStorage(0);
    rng = cvRNG(-1);

}


/***********************************
 * 函  数  名：paintEvent(QPaintEvent *event)
 * 功        能：paintEvent函数
 * 参        数：QPaintEvent *event
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
//    QPixmap fitPixmap = pixmap.scaled(width(),height(), Qt::KeepAspectRatio);
//    painter.drawPixmap(0, 0, fitPixmap);
    if(pic)
    {
        if( video_or_pic )
        {
    ////        rs = vd->get_frame((void **)&p,&len);
    ////        convert_yuv_to_rgb_buffer(p,pp,640,480/*QWidget::width(),QWidget::height()*/);
    ////        frame->loadFromData((uchar *)pp,/*len*/640 * 480 * 3 * sizeof(char));
            painter.drawImage(0,0,*frame);
    //        rs = vd->unget_frame();
        }
        else
        {
    //        frame->save("picture.bmp");
    //        qDebug("save pic!\n");
    //        image.load("picture.bmp");
            painter.drawImage(0,0,*frame);
            painter.setPen(QPen(Qt::blue,4));

            if(!list.empty())
            {
                painter.drawLines(list);
            }
            if(!list1.empty())
            {
                painter.drawLines(list1);
            }
            if(!list2.empty())
            {
                painter.drawLines(list2);
            }
            if(!list3.empty())
            {
                painter.drawLines(list3);
            }
        }
    }
    else
    {
        painter.drawImage(0,0,image);
    }
//    image.load(set_pic_filename_path);
//    painter.drawImage(0,0,image,image.width(),image.height());
//      painter.drawImage(0,0,image);
//在选取的点上标号或圈出
//    painter.setPen(QPen(Qt::red,1));
//    painter.drawEllipse(x-3,y-3,6,6);
}

/***********************************
 * 函  数  名：mousePressEvent(QMouseEvent *event)
 * 功        能：mousePressEvent函数，鼠标按下，记录位置event->x()，event->y()
 * 参        数：QMouseEvent *event
 * 返  回  值：无
 * ***********************************/
void MyLable::mousePressEvent(QMouseEvent *event)
{
    /*      contral mousemoveevent on or off    */
    setMouseTracking(true);
    if(event->button() == Qt::RightButton)
    {
        //获取点
        x=event->x();
        y=event->y();
        qDebug("x:%d",x);
        qDebug("y:%d\n",y);

        prev_pt = cvPoint(event->x(),event->y());
        update();
    }
}

/***********************************
 * 函  数  名：mouseMoveEvent(QMouseEvent *event)
 * 功        能：mouseMoveEvent函数，鼠标移动，记录鼠标用的路径
 * 参        数：QMouseEvent *event
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::mouseMoveEvent(QMouseEvent *event)
{
    /*      contral mousemoveevent on or off    */
    //setMouseTracking(true);

    //把鼠标移动的路径记录在QPoint里面
    QPoint x_y;
    x_y.setX(event->x());
    x_y.setY(event->y());
    switch (num_list) {
    case 0:
        list.append(x_y);
        break;
    case 1:
        list1.append(x_y);
        break;
    case 2:
        list2.append(x_y);
        break;
    case 3:
        list3.append(x_y);
        break;
    default:
        break;
    }

        CvPoint pt = cvPoint(event->x(),event->y());
        if( prev_pt.x < 0 )
        {
            prev_pt = pt;
        }
        //CvScalar 成员：double val[4] RGBA值A=alpha
        cvLine( marker_mask, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );
        mouseMove = true;
        cvLine( draw_img, prev_pt, pt, cvScalarAll(255), 5, 8, 0 );
        prev_pt = pt;
//        cvShowImage("draw_img",draw_img);

    update();
}

/***********************************
 * 函  数  名：mouseReleaseEvent(QMouseEvent *event)
 * 功        能：mouseMoveEvent函数，鼠标松开，
 *                    记录鼠标松开的位置，以此来判断完成一次鼠标事件
 * 参        数：QMouseEvent *event
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::mouseReleaseEvent(QMouseEvent *event)
{
    /*      contral mousemoveevent on or off    */
    setMouseTracking(false);
    if(event->button() == Qt::LeftButton)
    {
        num_list = num_list + 1;
        if(num_list == 4)
            num_list = 0;
    }

    prev_pt = cvPoint(-1,-1);
    update();
}

/***********************************
 * 函  数  名：set_filename()
 * 功        能：
 *
 * 参        数：
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::set_filename()
{
    pic = true;
    video_or_pic = true;
    mouseMove = false;
}

/***********************************
 * 函  数  名：set_filename()
 * 功        能：
 *
 * 参        数：
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::set_scaled(QVector<QPoint> list, int num)
{

}

/***********************************
 * 函  数  名：set_scaled_button()
 * 功        能：1)绑定手动分割控件
 *                       2)根据手动给出的分水岭种子分割
 *                          3)分水岭分割
 *
 * 参        数：
 * 返  回  值：无
 *
 * ***********************************/
int MyLable::set_scaled_button()
{
    int i, j, comp_count = 0;
    CvMat* color_tab = 0;
    CvSeq* contours = 0;

    /*判断 marker_mask 是否有数据*/
    if(!mouseMove)
    {
        QMessageBox::information(NULL, "提示", "请先画出要分割区域", QMessageBox::Yes, QMessageBox::Yes);
        return 1;
    }

/************************************************/
/*     k均值聚类，输入原图像，返回聚类结果     */
//    cvZero(marker_mask);
//    kmeans(img0,marker_mask);
//    cvThreshold(marker_mask,marker_mask,125,255,1);
cvShowImage("marker_mask_2zhi",marker_mask);
/************************************************/
    /*获取种子轮廓*/
    cvFindContours( marker_mask, storage, &contours, sizeof(CvContour),
                    CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    cvZero( markes );
     /*画出种子轮廓*/
    for( ; contours != 0; contours = contours->h_next, comp_count++ )
    {
        //cvDrawContours(markers, contours, cvScalarAll(comp_count+1),
            //cvScalarAll(comp_count+1), -1, -1, 8, cvPoint(0,0) );
        cvDrawContours(markes, contours, cvScalarAll(comp_count+1),
        cvScalarAll(comp_count+1), 1, -1, 8, cvPoint(0,0) );
    }

    //创建随机颜色列表
    color_tab = cvCreateMat( 1, comp_count, CV_8UC3 );
    for( i = 0; i < comp_count; i++ )	//不同的整数标记
    {
        uchar* ptr = color_tab->data.ptr + i*3;
        ptr[0] = (uchar)(cvRandInt(&rng)%180 + 50);
        ptr[1] = (uchar)(cvRandInt(&rng)%180 + 50);
        ptr[2] = (uchar)(cvRandInt(&rng)%180 + 50);
    }
    /* t 为测算分水岭算法用的时间*/
    double t = (double)cvGetTickCount();
    cvSave("img0.xml",markes);

     /*分水岭算法，img0为原图，markes为种子图*/
    cvWatershed( img0, markes );
    cvSave("img1.xml",markes);

    t = (double)cvGetTickCount() - t;
    printf( "exec time = %gms\n", t/(cvGetTickFrequency()*1000.) );

    /*********/
    for( i = 0; i < markes->height; i++ )
        for( j = 0; j < markes->width; j++ )
        {
            /*marks储存着边界性息*/
            int idx = CV_IMAGE_ELEM( markes, int, i, j );//markers的数据类型为IPL_DEPTH_32S
qDebug()<<idx;
             /*wshed是分割好的区域性息，可以查看各个区域*/
            uchar* dst = &CV_IMAGE_ELEM( wshed, uchar, i, j*3 );//BGR三个通道的数是一起的,故要j*3

             /*tongue_body用来储存舌体部分，* 根据markes区域和img0原图提取舌体*/
            uchar* t_body = &CV_IMAGE_ELEM( tongue_body, uchar, i, j*3 );

            /*tongue_body用来储存舌体部分，* 根据markes区域和img0原图提取舌体*/
           uchar* t_body_1 = &CV_IMAGE_ELEM( tongue_body_1, uchar, i, j*3 );

//           /*tongue_body用来储存舌体部分，* 根据markes区域和img0原图提取舌体*/
//          uchar* t_body_2 = &CV_IMAGE_ELEM( tongue_body, uchar, i, j*3 );

            /*t_body_bw同t_body，用来计算舌体外接矩和面积等*/
            uchar* t_body_bw = &CV_IMAGE_ELEM( tongue_body_bw, uchar, i, j*3 );

            /*原图，把舌体部分像素复制给 t_body */
            uchar* src_pic = &CV_IMAGE_ELEM( img0, uchar, i, j*3 );

            if( idx == -1 ) //输出时若为-1，表示各个部分的边界
            {
                //dst[0] = dst[1] = dst[2] = (uchar)128;
                dst[2] = (uchar)0;
                //把其余部分变成白色
                t_body[0] = (uchar)255;
                t_body[1] = (uchar)255;
                t_body[2] = (uchar)255;

                t_body_1[0] = (uchar)0;
                t_body_1[1] = (uchar)0;
                t_body_1[2] = (uchar)0;
            }
            else if( idx <= 0 || idx > comp_count )  //异常情况
            {
                dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here
            }
            else if( idx == 1 )  //异常情况  此处为舌体部分
            {
                //green      first
                dst[0] = (uchar)0;
                //显示此部分，把原图像素复制给 t_body
                t_body[0] = src_pic[0];
                t_body[1] = src_pic[1];
                t_body[2] = src_pic[2];
                //把其余部分变成白色
                t_body_bw[0] = (uchar)255;
                t_body_bw[1] = (uchar)255;
                t_body_bw[2] = (uchar)255;

//                t_body_1[0] = (uchar)255;
//                t_body_1[1] = (uchar)255;
//                t_body_1[2] = (uchar)255;
                t_body_1[0] = src_pic[0];
                t_body_1[1] = src_pic[1];
                t_body_1[2] = src_pic[2];
            }
            else if( idx == 2 )  //异常情况
            {
                //blue        second
                dst[1] = (uchar)255;
                //把其余部分变成白色
                t_body[0] = (uchar)255;
                t_body[1] = (uchar)255;
                t_body[2] = (uchar)255;

                t_body_1[0] = (uchar)255;
                t_body_1[1] = (uchar)255;
                t_body_1[2] = (uchar)255;
//                t_body_1[0] = src_pic[0];
//                t_body_1[1] = src_pic[1];
//                t_body_1[2] = src_pic[2];
            }
            else if( idx == 3 )  //异常情况
            {
                //red         third
                dst[2] = (uchar)255;
                //把其余部分变成白色
                t_body[0] = (uchar)255;
                t_body[1] = (uchar)255;
                t_body[2] = (uchar)255;

                t_body_1[0] = (uchar)255;
                t_body_1[1] = (uchar)255;
                t_body_1[2] = (uchar)255;
            }
            else //正常情况
            {
                uchar* ptr = color_tab->data.ptr + (idx-1)*3;
                dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];
            }
        }

//    cvShowImage("img_gray",img_gray);
//    cvShowImage("wshed",wshed);
//    cvAddWeighted( wshed, 0.5, img_gray, 0.5, 0, wshed );
//                wshed.x.y=0.5*wshed.x.y+0.5*img_gray+0加权融合图像
//                cvShowImage("swhed",wshed);
//                cvShowImage("img_gray",img_gray);
                //cvShowImage( "watershed transform", wshed );
                //cvShowImage("img_final",img_final);
    cvShowImage( "watershed transform", wshed );
    cvShowImage( "tongue_body_1", tongue_body_1 );
//    cvSaveImage("bw.jpg",tongue_body_bw);

/********************外接矩形**************/
        Mat src_gray;
        Mat threshold_output;
        std::vector<std::vector<Point> > contours_xx;
        vector<Vec4i> hierarchy;
    RNG rng(12345);

    double s_width,s_hight, s_s, s_l;

        cvtColor(Mat(tongue_body_bw,false),src_gray,CV_BGR2GRAY);
        threshold( src_gray, threshold_output, 125, 255, THRESH_BINARY );

        findContours( threshold_output, contours_xx, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


        vector<RotatedRect> minRect( contours_xx.size() );
//         Mat drawing_xg = Mat::zeros( src.size(), CV_8UC3 );
       /// 计算矩
       vector<Moments> mu(contours_xx.size() );

       for( int i = 0; i < contours_xx.size(); i++ )
       {
           minRect[i] = minAreaRect( Mat(contours_xx[i]) );
           mu[i] = moments( contours_xx[i], false );
        }
        for( int i = 0; i< contours_xx.size(); i++ )
           {
            s_s = contourArea(contours_xx[i]);
            s_l = arcLength( contours_xx[i], true );
      qDebug(" * Contour[%d] - Area (M_00) = %.2f - Area OpenCV: %.2f - Length: %.2f \n", i, mu[i].m00, contourArea(contours_xx[i]), arcLength( contours_xx[i], true ) );
      //       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      //       drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
      //       circle( drawing, mc[i], 4, color, -1, 8, 0 );

      Point2f rect_points[4];
      minRect[i].points( rect_points );
      Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
             for( int j = 0; j < 4; j++ )
             {
                 //画出矩形的边
                 //这有问题
//                line((Mat(tongue_body_bw,false)), rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
                cvLine(tongue_body_bw, rect_points[j], rect_points[(j+1)%4], color, 1, 8);
                //打印4个点
                  qDebug("x[%d]:%f;  y[%d]:%f",j, rect_points[j].x, j, rect_points[j].y);
             }
             s_width = qSqrt( qPow(rect_points[0].x-rect_points[1].x ,2) + qPow( rect_points[0].y-rect_points[1].y, 2));
             qDebug("wwww:%f",s_width);
             s_hight = qSqrt(qPow((rect_points[1].x-rect_points[2].x),2)+qPow(rect_points[1].y-rect_points[2].y,2));
             qDebug("hhhh:%f",s_hight);

           }
        emit send_s_l(s_width, s_hight, s_s, s_l);
        cvShowImage("tongue_body_bw",tongue_body_bw);

  /*************************************/
    //把t_body舌体转换到image以便显示
    img = Function::CjwIplToQImg(tongue_body);
    image = *img;

    /*bool函数，用来控制是显示视频流还是显示处理后的图片*/
    pic = false;

    cvReleaseMat( &color_tab );
//        cvReleaseImage(&g_pGrayImage);
//        cvReleaseImage(&g_pBinaryImage);
    update();
    clear_list();
}

/***********************************
 * 函  数  名：clear_list()
 * 功        能：
 *
 * 参        数：
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::clear_list()
{
    list.clear();
    list1.clear();
    list2.clear();
    list3.clear();
}


/***********************************
 * 函  数  名：closeEvent(QCloseEvent *event)
 * 功        能：
 *
 * 参        数：
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::closeEvent(QCloseEvent *event)
{
        printf("close\n");
        event->accept();  //exit
}

/***********************************
 * 函  数  名：show_picture()
 * 功        能：1)载入拍摄的图片并进行显示，
 *                      2)初始化后面需要用到的结构
 *                         3)绑定拍照控件
 * 参        数：无
 * 返  回  值：无
 *
 * ***********************************/
void MyLable::show_picture()
{
    //
    qDebug("show pic start!\n");
    video_or_pic = false;

    //保存图片
    frame->save("picture.bmp");
    qDebug("save pic!\n");

 /*****/
    //*  clear  */
    clear_list();
/*********************自动处理********************************/
    img0 = cvLoadImage("picture.bmp",1);
    /* 中值滤波*/
    Mat medianblur_src(img0,0);
    medianBlur(medianblur_src,medianblur_src,3);

    /*图像锐化-手动分水岭不用*/
//    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
//    cv::filter2D(medianblur_src, medianblur_src, medianblur_src.depth(), kernel);

    img = Function::CjwIplToQImg(img0);

    /*此处可以加图像预处理函数*/
//    Mat resutl;
//    cv::medianBlur(image,resutl,3);
    image = *img;

    markes = cvCreateImage(cvSize(image.width(),image.height()),IPL_DEPTH_32S, 1 );
    marker_mask = cvCreateImage( cvSize(image.width(),image.height()), 8, 1 );

    wshed = cvCloneImage( img0 );
    img_gray = cvCloneImage( img0 );
    draw_img = cvCloneImage( img0 );
    tongue_body = cvCloneImage( img0 );
        tongue_body_1 = cvCloneImage( img0 );
    tongue_body_bw = cvCloneImage(img0);

    cvCvtColor( img0, marker_mask, CV_BGR2GRAY );
    cvCvtColor( marker_mask, img_gray, CV_GRAY2BGR );

    cvZero(markes);
    cvZero(marker_mask);
    cvZero( wshed );
    cvZero(tongue_body);
        cvZero(tongue_body_1);
    cvZero(tongue_body_bw);

    num = 0;
  /*****/
//auto_show_pic();

    /*update函数触发painter，显示图片*/
    update();
    qDebug("show pic end!\n");
}


void MyLable::setPixmap(const uchar* buf)
{
//    frame->loadFromData(buf,/*len*/640 * 480 * 3 * sizeof(char));

//        pixmap.loadFromData(buf, /*len*/640 * 480 * 3 * sizeof(char), "bmp", NULL);
//        qDebug("ssss\n");
        update();
}


/***************************************
*
* 连接自动提取舌体控件
*
*
******************************************/
void MyLable::auto_show_pic_btn()
{
//    auto_show_pic();
    auto_kmeans();
    update();
}
/***************************************
*
* 自动提取舌体函数
*
*
******************************************/
void MyLable::auto_kmeans()
{
qDebug("kmeans start\n");
        cvZero(marker_mask);

        kmeans(img0,marker_mask);
qDebug("kmeans end!!!");
        mouseMove = true;


        cvThreshold(marker_mask,marker_mask,125,255,1);
                cvNot(marker_mask,marker_mask);
        cvShowImage("marker_mask_auto",marker_mask);
//    cvShowImage("marker_mask_2zhi",marker_mask);
qDebug("water start!!!\n");
        set_scaled_button();
qDebug("water end!!!\n");

}


void MyLable::auto_show_pic()
{
//    IplImage* img0;
//    img0 = cvLoadImage("2.jpg");
/*rgb 转换 hsv*/
    hsv = cvCreateImage(cvGetSize(img0),8,3);//给hsv色系的图像申请空间
    hue = cvCreateImage(cvGetSize(img0),8,1);  //色调
    saturation = cvCreateImage(cvGetSize(img0),8,1);//饱和度
    value = cvCreateImage(cvGetSize(img0),8,1);//亮度

    cvCvtColor(img0,hsv,CV_BGR2HSV);//将RGB色系转为HSV色系

    cvSplit(hsv, hue, 0, 0, 0 );//分离三个通道
    cvSplit(hsv, 0, saturation, 0, 0 );
    cvSplit(hsv, 0, 0, value, 0 );

/*对h和v进行全局阈值处理*/
    int th1 = 100;//全局阈值  二值化
    cv::Mat global1;
    cv::threshold(cv::Mat(hue,false),global1,th1,255,CV_THRESH_BINARY_INV);

    int th2 = 80;
    cv::Mat global2;
    cv::threshold(cv::Mat(value,false),global2,th2,255,CV_THRESH_BINARY_INV);

/*取hue图像的上部份，取value的下部分*/
    for (int i = 0; i <(global2.rows)/2; i++)
    {
        for(int j = 0; j < global2.cols; j++)
        {
            for(int n = 0; n < global2.channels(); n++ )
            {
                if(global2.at<uchar>(i,j*global2.channels()+n) > 100)
                {
                    global1.at<uchar>(i,j*global1.channels()+n) = 255;
                }
            }
        }
    }
    cv::imshow("glo",global1);

/*先膨胀在腐蚀*/
    IplImage *img_erode = cvCreateImage(cvGetSize(img0), 8, 1);
    IplImage *img_dilate = cvCreateImage(cvGetSize(img0), 8, 1);

    IplImage imgtmp = global1;//mat to iplimage*
    IplImage *input = cvCloneImage(&imgtmp);

    cvErode( input,img_erode, NULL,1); //腐蚀
    cvDilate( img_erode,img_dilate, NULL,1); //膨胀
    cvShowImage("腐蚀",img_erode);
    cvShowImage("膨胀",img_dilate);

    IplImage* dst = NULL;
CvMemStorage* storage = cvCreateMemStorage (0);
    CvSeq* contour = 0;
    double contour_area_temp = 0, contour_area_max = 0, contour_area_2 = 0;
    CvSeq* area_max_contour = 0;
    CvSeq* area_2_contour = 0;
    int contours = 0;
    CvScalar external_color;
    CvScalar hole_color;
    //寻找轮廓   CV_RETR_EXTERNAL     CV_RETR_LIST
contours = cvFindContours (img_dilate, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
qDebug("contours:%d",contours);

//创建新的图像显示轮廓
dst = cvCreateImage (cvGetSize(img0), img0->depth, img0->nChannels);
cvCopy(img0,dst);

for (;contour != 0; contour = contour->h_next)
{
    contour_area_temp = fabs(cvContourArea(contour,CV_WHOLE_SEQ));
    if(contour_area_temp > contour_area_max)
    {
        contour_area_2 = contour_area_max;
        contour_area_max = contour_area_temp;
        area_2_contour = area_max_contour;
        area_max_contour = contour;
    }
}
    external_color = CV_RGB(rand()&255, rand()&255, rand()&255);
   hole_color = CV_RGB(rand()&255, rand()&255, rand()&255);
   //画轮廓
//    cvDrawContours (dst, area_2_contour, external_color, hole_color, 0, 2, 8);
    cvDrawContours (dst, area_max_contour, external_color, hole_color, 0, 2, 8);

//   cvNamedWindow ("Contour", 1);
   cvShowImage ("Contour", dst);

   for (int i = 0; i < img0->height; i++)
       {
           for(int j = 0; j < img0->width; j++)
           {
               //-1 0 1分别代表轮廓外，轮廓上，轮廓里面
               if(-1 == (cvPointPolygonTest(area_2_contour,cv::Point2f(j,i),0)))
               {
                   ((uchar *)(img0->imageData +  i * img0->widthStep ))[j * img0->nChannels + 0] = 234;   //改变该像素B的颜色分量
                   ((uchar *)(img0->imageData +  i * img0->widthStep ))[j * img0->nChannels + 1] = 200;   //改变该像素G的颜色分量
                   ((uchar *)(img0->imageData +  i * img0->widthStep ))[j * img0->nChannels + 2] = 150;   //改变该像素R的颜色分量
               }
           }
       }
   cv::imshow("final",cv::Mat(img0,false));


   cvReleaseImage(&dst);
   cvReleaseMemStorage(&storage);
   cvReleaseImage(&img_dilate);
   cvReleaseImage(&img_erode);

/*****************************************/
}


MyLable::~MyLable()
{
    rs = vd->stop_capturing();
    rs = vd->uninit_device();
    rs = vd->close_device();
    cvReleaseImage(&hsv);
    cvReleaseImage(&hue);
    cvReleaseImage(&value);
    cvReleaseImage(&saturation);
}

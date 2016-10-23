#include "qkmeas.h"
using namespace cv;
using namespace std;

void kmeans(IplImage* src,IplImage* resImg)
{
    IplImage* lab;
    lab = cvCreateImage(cvGetSize(src),8,3);//给hsv色系的图像申请空间
    cvCvtColor(src,lab,CV_BGR2Lab);

    /****************************************/
    IplImage *dst;
     IplImage *dst1;
     IplImage *dst2;
     IplImage *dst3;

     dst1 = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
     dst2 = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
     dst3 = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
     dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 3);

     cvSplit(lab, dst1, dst2, dst3, 0);//分离RGB通道，分别得到BGR的单通道图像
      cvMerge(0, dst2, dst3, 0, dst);//逆运算，将三通道图像融合得到彩色图像
/******************************************************/

cvShowImage("dst",dst);
    int total= src->height*src->width;
    int cluster_num = 3;
    CvMat *row = cvCreateMat( src->height,src->width,CV_32FC3 );
    cvConvert(dst,row);//转一下类型！
    CvMat *clusters = cvCreateMat( total, 1, CV_32SC1 );
    cvReshape(row,row,3,total);//修改矩阵的形状,每个数据一行，使row指向修改后的数据，不修改通道数
    cvKMeans2( row, cluster_num, clusters,cvTermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 10/* 迭代次数*/, 1.0/*迭代误差*/ ));
    cvReshape(clusters,clusters,3,dst->width);//聚类完的结果再reshape回来方便看

    int i=0,j=0;
    CvScalar s;
//        /*IplImage* */resImg = cvCreateImage( cvSize(src->width,src->height), 8, 1 );//生成用来显示结果的图像
    IplImage* img1 = cvCreateImage( cvSize(src->width,src->height), 8, 1 );//生成用来显示结果的图像
    IplImage* img2 = cvCreateImage( cvSize(src->width,src->height), 8, 1 );//生成用来显示结果的图像
    IplImage* img3 = cvCreateImage( cvSize(src->width,src->height), 8, 1 );//生成用来显示结果的图像

    s=cvGet2D(src,i,j);
    for(i=0;i<src->height;i++)
    {
        for (j=0;j<src->width;j++)
        {
            if (clusters->data.i[i*src->width+j]==1)
            {
                s.val[0]=255;
                s.val[1]=255;
                s.val[2]=255;
//                cvSet2D(resImg,i,j,s);//注意循环顺序
                cvSet2D(img1,i,j,s);
            }
            else
            {
                if (clusters->data.i[i*src->width+j]==2)
                {
                    s.val[0]=155;
                    s.val[1]=155;
                    s.val[2]=155;
//                        cvSet2D(resImg,i,j,s);//注意循环顺序
                    cvSet2D(img2,i,j,s);
                }
                else
                {
                    s.val[0]=55;
                    s.val[1]=55;
                    s.val[2]=55;
//                    cvSet2D(resImg,i,j,s);
                    cvSet2D(img3,i,j,s);
                }
            }
        }
    }

    /*显示分割的部分  */
    cvShowImage( "img1", img1 );
    cvShowImage( "img2", img2);
    cvShowImage( "img3", img3 );

    IplImage *img1_dst = cvCreateImage(cvGetSize(src), 8, 1);
    chuli(img1,img1_dst);
    cvShowImage( "img1_chuli", img1_dst );
    IplImage *img2_dst = cvCreateImage(cvGetSize(src), 8, 1);
    chuli(img2,img2_dst);
    cvShowImage( "img2_chuli", img2_dst);
    IplImage *img3_dst = cvCreateImage(cvGetSize(src), 8, 1);
    chuli(img3,img3_dst);
    cvShowImage( "img3_chuli", img3_dst );


    //合并两个图像
//    IplImage *img_hebing = cvCreateImage(cvGetSize(src), 8, 1);
    cvAddWeighted(img1_dst,0.5,img2_dst,0.5,0.0,resImg);

//    cvNot(resImg,resImg);

    cvShowImage( "img1_and_img2", resImg );

    cvReleaseMat(&row);
    cvReleaseMat(&clusters);

    cvReleaseImage (&img1);
    cvReleaseImage (&img2);
    cvReleaseImage (&img3);
    cvReleaseImage (&lab);
    cvReleaseImage (&dst);
    cvReleaseImage (&dst1);
    cvReleaseImage (&dst2);
    cvReleaseImage (&dst3);
    cvReleaseImage (&img1_dst);
    cvReleaseImage (&img2_dst);
    cvReleaseImage (&img3_dst);
    cvReleaseImage (&img1);
}

/**去掉过小的洞，并腐蚀膨胀***/
void chuli(IplImage* src,IplImage *dst)
{
//    CvSeq* contour = 0;
////        IplImage* dst = cvCreateImage(cvGetSize(src), 8, 1);
//CvMemStorage* storage = cvCreateMemStorage(0);

//    cvFindContours( src, storage, &contour, sizeof(CvContour),
//                    CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
//    cvZero(dst);        // 清空数组

//    double maxarea = 5000;
//    double minarea = 4000;
//    int m = 0;
//    for( ; contour != 0; contour = contour->h_next )
//    {

//        double tmparea = fabs(cvContourArea(contour));
//        if(tmparea < minarea)
//        {
//            cvSeqRemove(contour, 0); // 删除面积小于设定值的轮廓
//            continue;
//        }
//        CvRect aRect = cvBoundingRect( contour, 0 );
//        if ((aRect.width/aRect.height)<1)
//        {
//            cvSeqRemove(contour, 0); //删除宽高比例小于设定值的轮廓
//            continue;
//        }
//        if(tmparea > maxarea)
//        {
//            maxarea = tmparea;
//        }
//        m++;
//        // 创建一个色彩值
//        CvScalar color = CV_RGB( 0, 255, 255 );

//        //max_level 绘制轮廓的最大等级。如果等级为0，绘制单独的轮廓。如果为1，绘制轮廓及在其后的相同的级别下轮廓
//        //如果值为2，所有的轮廓。如果等级为2，绘制所有同级轮廓及所有低一级轮廓，诸此种种
//        //如果值为负数，函数不绘制同级轮廓，但会升序绘制直到级别为abs(max_level)-1的子轮廓
//        cvDrawContours(dst, contour, color, color, -1, 1, 8);   //绘制外部和内部的轮廓
//        cvDrawContours(dst, contour, CV_RGB(255, 255, 255),
//                       CV_RGB(255, 255, 255), 0, CV_FILLED, 8, cvPoint(0, 0));   //绘制外部和内部的轮廓
//    }
/***************************/
    max_liantong(src,dst);

/***************************/
//        /*先膨胀在腐蚀*/
//            IplImage *img_erode = cvCreateImage(cvGetSize(dst), 8, 1);
        IplImage *img_dilate = cvCreateImage(cvGetSize(dst), 8, 1);

//       cvDilate( dst,img_dilate, NULL,1); /
        cvErode( dst,dst, NULL,1); //腐蚀
        cvDilate( dst,dst, NULL,2);
        cvErode( dst,dst, NULL,3); //腐蚀
        IplImage * temp = cvCreateImage(cvGetSize(dst), 8,1);
//cvMorphologyEx(dst,dst,temp,NULL,CV_MOP_OPEN,1);
//            cvShowImage("腐蚀",img_erode);
//        cvShowImage("膨胀",img_dilate);


    cvNamedWindow("Components", 1);
//    cvShowImage("Components", dst);
//        cvWaitKey(0);
    cvDestroyWindow("Source");
//        cvReleaseImage(&img);
    cvDestroyWindow("Components");
//        cvReleaseImage(&dst);
//        cvReleaseImage(&img_erode);
    cvReleaseImage(&img_dilate);
        cvReleaseImage(&temp);
}

//求最大联通区域
void max_liantong(IplImage* src,IplImage *dst)
{
        cvNamedWindow("origin");
        cvShowImage("origin", src); // 显示原始图像
        // 摄像机保存的图像是32位的，有R,G,B和Alpha通道
        // 图像中实际存储顺序是B,G,R

        cvThreshold(src, dst, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);   // OTSU法二值化
        {
            IplConvKernel *element = cvCreateStructuringElementEx(5, 5, 0, 0, CV_SHAPE_ELLIPSE); // 定义形态学结构指针
            cvMorphologyEx(dst, dst, NULL, element, CV_MOP_OPEN);       // 开运算,去除比结构元素小的亮点
            cvReleaseStructuringElement(&element);
        }
        cvNamedWindow("binary");
        cvShowImage("binary", dst);   // 显示二值化图像
        {
            int color = 254;            // 从254开始，因此连通域不能多于253个
            CvSize sz = cvGetSize(dst);
            int w, h;
            for (w = 0; w < sz.width; w++)
            {
                for (h = 0; h < sz.height; h++)
                {
                    if (color > 0)
                    {
                        if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == 255)
                        {
                            cvFloodFill(dst, cvPoint(w, h), CV_RGB( color,color,color)); // 把各连通域标记上颜色
                            color--;
                        }
                    }
                }
            }
            cvNamedWindow("labeled");
            cvShowImage("labeled", dst);  // 显示标记后的图像
            int colorsum[255] = {0};
            for (w = 0; w < sz.width; w++)
            {
                for (h=0; h<sz.height; h++)
                {
                    if (CV_IMAGE_ELEM(dst, unsigned char, h, w) > 0)   // 不对0值计数，不可能为255
                    {
                        colorsum[CV_IMAGE_ELEM(dst, unsigned char, h, w)]++; // 统计每种颜色的数量
                    }
                }
            }
            std::vector<int> v1(colorsum, colorsum + 255);    // 用数组初始化vector
            int maxcolorsum = max_element(v1.begin(), v1.end()) - v1.begin(); // 求出最多数量的颜色
            for (w = 0; w < sz.width; w++)
            {
                for (h = 0; h < sz.height; h++)
                {
                    if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == maxcolorsum)
                    {
                        CV_IMAGE_ELEM(dst, unsigned char, h, w) = 255;    // 只把最多数量的颜色标为255
                    }
                    else
                    {
                        CV_IMAGE_ELEM(dst, unsigned char, h, w) = 0;      // 其他标为0
                    }
                }
            }
            cvNamedWindow("最大连通域");
            cvShowImage("最大连通域", dst);    // 显示最大连通域
        }
//        cvReleaseImage(&src);
}

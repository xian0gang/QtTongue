#include "widget.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //set chinese
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//    QTextCodec::setCodecForTr(codec);
//    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
//    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
//        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));       //支持Tr中文
//        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));//支持中文文件名显示
//        QFont font;
//        font.setPointSize(160);//实际上是16的字号，但设成16却不行
//        font.setFamily(("wenquanyi"));
//        font.setBold(false);


    Widget w;
    w.show();
//w.setFont(font);
    return a.exec();
}

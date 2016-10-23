#include "showimg.h"
#include "ui_showimg.h"

ShowImg::ShowImg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowImg)
{
    ui->setupUi(this);
}

ShowImg::~ShowImg()
{
    delete ui;
}

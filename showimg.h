#ifndef SHOWIMG_H
#define SHOWIMG_H

#include <QWidget>

namespace Ui {
class ShowImg;
}

class ShowImg : public QWidget
{
    Q_OBJECT

public:
    explicit ShowImg(QWidget *parent = 0);
    ~ShowImg();

private:
    Ui::ShowImg *ui;
};

#endif // SHOWIMG_H

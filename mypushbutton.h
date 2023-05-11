#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
//    explicit MyPushButton(QWidget *parent = 0);

    //为了兼容鼠标按下时按钮向下跳动和变化图标两种情况，重写带参的构造函数
    //构造函数 参数1 正常显示的图片路径 参数2 按下后显示的图片路径
    MyPushButton(QString normalImg,QString pressImg = "");

    //成员属性:

    //保存用户传入的默认显示路径
    QString normalImgPath;
    //按下后显示的另一张图片路径
    QString pressImgPath;



    //弹跳特效
    void zoom_up();//向下跳
    void zoom_down();//向上跳

    //重写按钮的 按下 和 释放 事件
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);


signals:

public slots:

};

#endif // MYPUSHBUTTON_H

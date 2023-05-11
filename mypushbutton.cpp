#include "mypushbutton.h"
#include<QDebug>
#include<QPropertyAnimation>

//MyPushButton::MyPushButton(QWidget *parent) :
//    QPushButton(parent)
//{
//}

MyPushButton::MyPushButton (QString normalImg,QString pressImg)
{
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;

    QPixmap pix;
    bool ret = pix.load(normalImg);
    if(!ret){
        qDebug()<<"图片加载失败";
        return;
    }


    //设置图片固定大小
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));





}

//向下跳
void MyPushButton::zoom_down()
{
    //创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    //设置动画时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();

}

//向上跳
void MyPushButton::zoom_up()
{
    //创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    //设置动画时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}


//重写按钮的 按下 和 释放 事件
void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    if(this->pressImgPath != "")//传入的图片不为空，说明按下时需要显示另一张图片
    {
        //qDebug()<<"切换图片！";
        //切换图片,即为重新用Pixmap渲染这个按钮

        QPixmap pix;
        bool ret = pix.load(this->pressImgPath);
        if(!ret){
            qDebug()<<"图片加载失败";
            return;
        }


        //设置图片固定大小
        this->setFixedSize(pix.width()*1.2,pix.height()*1.2);

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width()*1.2,pix.height()*1.2));

        //IMPOOOOOOOOOOOOOOOOOOOOOOORTANT!!!!!!
        //让父类执行其他内容
        //相当于这个mousePressEvent函数过滤了一下鼠标点击事件发生时事件的分发
        //将按钮图片渲染完成后还需要继续执行返回功能的代码，让父类执行其他内容
        return QPushButton::mousePressEvent(e);
    }
    else{
        //qDebug()<<"不用切换图片！！";
        return QPushButton::mousePressEvent(e);
    }
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->pressImgPath != "")//切换回初始的图片
    {
        //切换图片,即为重新用Pixmap渲染这个按钮
        //qDebug()<<"切换回初始的图片!";
        QPixmap pix;
        bool ret = pix.load(this->normalImgPath);
        if(!ret){
            qDebug()<<"图片加载失败";
            return;
        }


        //设置图片固定大小
        this->setFixedSize(pix.width(),pix.height());

        //设置不规则图片样式
        this->setStyleSheet("QPushButton{border:0px;}");

        //设置图标
        this->setIcon(pix);

        //设置图标大小
        this->setIconSize(QSize(pix.width(),pix.height()));

        //IMPOOOOOOOOOOOOOOOOOOOOOOORTANT!!!!!!
        //让父类执行其他内容
        //相当于这个mousePressEvent函数过滤了一下鼠标点击事件发生时事件的分发
        //将按钮图片渲染完成后还需要继续执行返回功能的代码，让父类执行其他内容
        return QPushButton::mouseReleaseEvent(e);
    }
    else{
        //qDebug()<<"切换回原按钮图片！";
        return QPushButton::mouseReleaseEvent(e);
    }
}

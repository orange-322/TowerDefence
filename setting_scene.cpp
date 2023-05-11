#include "setting_scene.h"
#include "ui_setting_scene.h"
#include<QDebug>
setting_scene::setting_scene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting_scene)
{
    //先建立ui（因为用到了ui设计的horizontal_slider）
    ui->setupUi(this);

    //volume信号函数的指针，用于发送信号
    void (QSlider::*volume_slider_signal)(int) = &QSlider::valueChanged;

    //检测到音量slider改变时发送信号给mainscene监听（带一个参数：音量）
    connect(ui->volume_slider,volume_slider_signal,this,[=](){
//        QString a = QString("volume = %1").arg(ui->volume_slider->value());
//        qDebug()<<a;
        emit this->volume_setting_signal(ui->volume_slider->value());
    });

    //设置图标
    this->setWindowIcon(QIcon(":/res/window_icon.png"));

    //设置标题
    this->setWindowTitle("设置！");

}

setting_scene::~setting_scene()
{
    delete ui;
}

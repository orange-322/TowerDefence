#ifndef CHOOSESTATESCENE_H
#define CHOOSESTATESCENE_H

#include <QMainWindow>
#include <QMediaPlayer>//多媒体模块下的音效头文件
#include"gamewindow.h"

class ChooseStateScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit ChooseStateScene(QWidget *parent = 0);

    //重写绘图事件
    void paintEvent(QPaintEvent *);

    //游戏场景对象指针
    GameWindow *game = nullptr;

    //关卡选择界面的bgm
    QMediaPlayer *choose_state_scene_bgm = nullptr;

    //开始播放bgm
    void play_bgm(){ choose_state_scene_bgm->play();}

    //停止播放bgm
    void stop_bgm(){ choose_state_scene_bgm->stop();}

    //关卡选择页面的设置界面
    setting_scene * choose_state_scene_settings = nullptr;

signals:
    //用于告诉主场景 点击了返回 的自定义信号
    void ChooseSceneBack();

public slots:

};

#endif // CHOOSESTATESCENE_H

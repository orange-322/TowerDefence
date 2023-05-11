#ifndef MAINSCENE_H
#define MAINSCENE_H

#include <QMainWindow>
#include<QWebView>//用于显示HTML文档的库
#include"choosestatescene.h"//关卡选择界面
//由于三个界面都需要设置界面，故把设置界面的include放在了最上游的gamewindow文件里了
#include "index_scene.h"//index显示界面


namespace Ui {
class MainScene;
}

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainScene(QWidget *parent = 0);
    ~MainScene();

    //重写paintEvent事件 画背景图
    void paintEvent(QPaintEvent *);

    //关卡选择界面
    ChooseStateScene *ChooseScene = nullptr;
    //主页面的设置界面
    setting_scene * main_settings = nullptr;
    //index显示界面
    index_scene * index_window = nullptr;

    QWebView *game_index;//游戏index，有介绍游戏玩法、显示游戏版本和功能的作用

private:
    Ui::MainScene *ui;
};

#endif // MAINSCENE_H

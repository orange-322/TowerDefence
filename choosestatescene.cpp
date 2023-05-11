#include "choosestatescene.h"
#include<QMenuBar>
#include<QPainter>

#include"mypushbutton.h"
#include"QDebug"
ChooseStateScene::ChooseStateScene(QWidget *parent) :
    QMainWindow(parent)
{
    //配置选择关卡场景
    //设置固定大小
    this->setFixedSize(738,600);

    //设置图标
    this->setWindowIcon(QIcon(":/res/window_icon.png"));

    //设置标题
    this->setWindowTitle("选择关卡！");

    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建[设置] 菜单项
    QAction * settingAction = startMenu->addAction("设置");

///[########]这里可以添加更多菜单项

    //创建[退出] 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击退出 实现退出游戏
    connect(quitAction,&QAction::triggered,this,[=](){
        this->close();
    });

    //设置背景音乐
    choose_state_scene_bgm = new QMediaPlayer(this);
    choose_state_scene_bgm->setMedia(QUrl::fromLocalFile("D:/Qt/my_own/chap02/TowerDefence/music/choose_state_scene.mp3"));
    choose_state_scene_bgm->setVolume(50);

    //实例化关卡选择页面的设置界面
    choose_state_scene_settings = new setting_scene;
    //点击菜单栏中的设置实现打开设置界面
    connect(settingAction,&QAction::triggered,this,[=](){
        choose_state_scene_settings->show();
    });
///////////////////////////////////////////////////设置界面的各项功能连接如下//////////////////////////////////////////////////////////////
    //监听来自设置界面的音量调节signal
    connect(choose_state_scene_settings,&setting_scene::volume_setting_signal,this,[=](int j){
        choose_state_scene_bgm->setVolume(j);
    });
///////////////////////////////////////////////////设置界面的各项功能连接如上//////////////////////////////////////////////////////////////

    /////////////////////////////////////////////

    //实现返回按钮
    MyPushButton * backBtn = new MyPushButton("://res/back_btn.png","://res/back_btn_.png");
    //加入对象树
    backBtn->setParent(this);
    //设置返回按钮的位置(后期可调整)
    //backBtn->move(this->width()-backBtn->width()-200,this->height()-backBtn->width()-150);
    backBtn->move(553,353);

    //点击返回
    connect(backBtn,&MyPushButton::clicked,this,[=](){
        qDebug()<<"点击了返回";
        //告诉主场景 我返回了 主场景监听ChooseStateScene的返回按钮
        //*******************************发送信号**********************，从而可以在界面直接传递消息
        //*******************************技巧性强**********************
        emit this->ChooseSceneBack();
    });


    //创建选择关卡的按钮
    QString prefix = "://res/stage";
    QString suffix = ".png";
    for(int i=1;i<10;i++){
        //设置图片路径和按下去显示的另一张图片的路径
        QString normal_path = prefix+QString::number(i)+suffix;
        QString press_path = prefix +QString::number(i)+"_"+suffix;
        //创建新按钮
        MyPushButton *menuBtn = new MyPushButton(normal_path,press_path);
        //连上爸爸
        menuBtn->setParent(this);
        //调好位置
        if(i<=5)
            menuBtn->move(93+(i-1)*115,220);
        else
            menuBtn->move(93+(i-6)*115,353);
        //监听每个按钮的点击事件（必须在for循环内连好）
        connect(menuBtn,&MyPushButton::clicked,this,[=](){
           QString str = QString("您选择的是第 %1 关").arg(i);
           qDebug()<<str;
           //进入游戏主界面
           //隐藏关卡选择界面
           this->hide();
           this->stop_bgm();//停止关卡选择界面bgm

           //创建游戏窗口界面并显示
           game = new GameWindow(i);//将关卡数传入GameWindow
           game->show();
           game->play_bgm();//播放游戏界面的bgm
           //(这里加上对每个窗口的返回按钮的监听connect，从而可以从游戏窗口返回选择关卡界面)
           //实现对关卡选择界面返回键的监听，从而关闭关卡选择界面，重新显示主界面
           connect(game,&GameWindow::GameWindowBack,this,[=](){
//               game->hide();
               game->stop_bgm();//停止播放bgm
               game->close();
               this->show();
               this->play_bgm();//重新播放bgm
           });


        });

    }


}


void ChooseStateScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load("://res/choose_state.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

}

#include "mainscene.h"
#include "ui_mainscene.h"
#include<QDebug>
#include<QPainter>
#include"mypushbutton.h"
#include<QTimer>
#include <QMediaPlayer>//多媒体模块下的音效头文件


#include"map_and_path.h"
#include<io.h>//用于判断文件是否存在
#include<QString>
#include<QByteArray>//用于QString和char*间的转换
#include"map_and_path.h"

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景

    //设置固定大小
    setFixedSize(1280,720);

    //设置图标
    setWindowIcon(QIcon(":/res/window_icon.png"));

    //设置标题
    setWindowTitle("元气塔防！");

    //设置背景音乐
    QMediaPlayer *mainscene_bgm = new QMediaPlayer(this);
    mainscene_bgm->setMedia(QUrl::fromLocalFile("D:/Qt/my_own/chap02/TowerDefence/music/mainscene.mp3"));
    mainscene_bgm->setVolume(50);
    mainscene_bgm->play();

    //设置index按钮
    MyPushButton * index_btn = new MyPushButton("://res/index_btn.png","://res/index_btn_.png");
    index_btn->setParent(this);
    index_btn->move(19,486);
    //实例化index界面
    index_window = new index_scene;
    connect(index_btn,&QPushButton::clicked,this,[=](){
        index_window->show();
    });
    //然后在index界面内写HTML文档的显示内容

    //设置setting按钮
    MyPushButton * main_setting_btn = new MyPushButton("://res/setting_btn.png","://res/setting_btn_.png");
    main_setting_btn->setParent(this);
    main_setting_btn->move(16,600);

    //实例化主页面的设置界面
    main_settings = new setting_scene;
    connect(main_setting_btn,&QPushButton::clicked,this,[=](){
        main_settings->show();
    });
///////////////////////////////////////////////////设置界面的各项功能连接如下//////////////////////////////////////////////////////////////
    //监听来自设置界面的音量调节signal
    connect(main_settings,&setting_scene::volume_setting_signal,this,[=](int j){
        mainscene_bgm->setVolume(j);
//        //test
//        QString a = QString("volume = %1").arg(j);
//        qDebug()<<a;
    });
///////////////////////////////////////////////////设置界面的各项功能连接如上//////////////////////////////////////////////////////////////
    //退出按钮实现
    connect(ui->actionQuit,&QAction::triggered,this,[=](){
        this->close();
    });

    //开始按钮
    MyPushButton * startBtn = new MyPushButton("://res/new_game_btn.png");
    startBtn->setParent(this);
    startBtn->move(this->width()*0.5-startBtn->width()*0.5,this->height()*0.7);

    //实例化选择关卡的场景
    ChooseScene = new ChooseStateScene;

    //************技巧性比较强，界面之间的切换用两个界面类的信号传递实现***************
    //从关卡选择界面返回主界面
    //实现对关卡选择界面返回键的监听，从而关闭关卡选择界面，重新显示主界面
    connect(ChooseScene,&ChooseStateScene::ChooseSceneBack,this,[=](){
        ChooseScene->hide();
        this->show();
        mainscene_bgm->play();//回到主界面，重新开始播放音乐
        ChooseScene->stop_bgm();
    });

    //监听开始按钮
    connect(startBtn,&MyPushButton::clicked,this,[=](){
        //qDebug()<<"点击了新游戏";

        //做弹起动画特效
        startBtn->zoom_down();
        startBtn->zoom_up();

        //进入选择关卡的页面
        //延时，开启一个局部的事件循环，让其执行400ms后自己退出(等待按钮弹起动画结束)
        QEventLoop eventloop;
        QTimer::singleShot(400,&eventloop,SLOT(quit()));
        eventloop.exec();


        //主窗口自身隐藏
        this->hide();
        mainscene_bgm->stop();//进入关卡选择界面，停止播放音乐
        ChooseScene->play_bgm();//播放关卡选择界面的背景音乐
        //显示选择关卡场景窗口
        ChooseScene->show();



    });

//////////////////////////////地图文件初始化//////////////////////////////////////////////
    //仅在第一次点开exe时初始化地图文件，后面再打开都是用现有文本，和硬编码的内容无关了
    //将map_and_path.h中的1-8关地图信息初始化到txt文件中
    if((_access("map1.txt",0)) == -1)//如果map1.txt不存在，说明所有地图都没初始化过，才用map_and_path.h中硬编码的地图来新建初始化地图文本文件
    {
        //qDebug()<<"map1.txt不存在";
        //用FILE操作，将第一关中的地图和路径硬编码写入文件
        FILE *f1 = fopen("map1.txt","w");
        if(f1 == nullptr)
            {qDebug()<<"Error!";}
        //将第一关地图数组硬编码写入文件
        fprintf(f1,"%d,%d\n",8,12);
        for(int i=0;i<MAP_ROW;i++)
        {
            for(int j = 0;j<MAP_COL;j++)
            {
                fprintf(f1,"%d,",all_state_map[0][i][j]);
            }
            fputc('\n',f1);
        }
        //将第一关路径硬编码写入文件
        Paths *paths = new Paths(1);
        QVector<QVector<location>> map1_path_set = paths->get_path_set();
        fprintf(f1,"%d\n",map1_path_set.size());
        for(int i=0;i<map1_path_set.size();i++)
        {
            fprintf(f1,"%d\n",map1_path_set[i].size());
            for(int j=0;j<map1_path_set[i].size();j++)
                fprintf(f1,"%d,%d\n",map1_path_set[i][j].x,map1_path_set[i][j].y);
        }
        fclose(f1);
    }

        //接下来用一个循环把2-8关初始化文件写好
        //2-8关初始化统一载入全是墙的地图
        //文件名的前缀和后缀，用于拼接出最终需要创建的文件名
        QString prefix = "map";
        QString surfix = ".txt";
        for(int StateNum = 2;StateNum<=9;StateNum++)
        {
            //利用QString和int/char*的转化以及QString的拼接，实现创建不同文件名(map2-map8)的地图文件
            QString temp_state_num = QString("%1").arg(StateNum);
            QString QS_map_file_path = prefix+temp_state_num+surfix;
            char *map_file_path;
            QByteArray ba = QS_map_file_path.toLatin1();
            map_file_path = ba.data();//至此第StateNum关的地图文件名就写好了

            //写入地图
            if((_access(map_file_path,0)) == -1)//如果当前这一关的地图文本文件不存在，就对其进行初始化（创建文件、写入硬编码）
            {
                FILE *fp = fopen(map_file_path,"w");
                fprintf(fp,"%d,%d\n",8,12);//8*12的地图大小
                for(int i=0;i<MAP_ROW;i++)
                {
                    for(int j=0;j<MAP_COL;j++)
                    {
                        fprintf(fp,"%d,",all_state_map[StateNum-1][i][j]);
                    }
                    fputc('\n',fp);
                }
                //写入路径
                fprintf(fp,"%d\n",0);//路径统一初始化为0
                fclose(fp);
            }
        }


}


void MainScene::paintEvent(QPaintEvent *)
{
    //画主界面背景图
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/MainScene.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);



}


MainScene::~MainScene()
{
    delete ui;
}

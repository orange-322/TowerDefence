#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include<QWidget>
#include<QPainter>
#include<QMouseEvent>
#include<QLabel>
#include <QMediaPlayer>//多媒体模块下的音效头文件

#include "setting_scene.h"
#include"enemy_1.h"
#include"enemy_2.h"
#include"enemy_3.h"
#include"enemy_4.h"
#include"tower_1.h"
#include"tower_2.h"
#include"tower_3.h"
#include"tower_4.h"
#include"tower_5.h"
#include"meleetowerselection.h"
#include"remotetowerselection.h"



//游戏窗口界面
class GameWindow : public QMainWindow
{
    Q_OBJECT

private:
    //成员变量：
    //*******************************************



    //显示当前游戏时间
    int GameTime = 0;//从游戏开始时计时
    QLabel* TimeLabel = new QLabel(this);//游戏时长标签
    QLabel* TimeIcon = new QLabel(this);//游戏时长图案
    QLabel* MoneyLabel = new QLabel(this);//生命标签
    QLabel* MoneyIcon = new QLabel(this);//生命图案
    QLabel* PlayerHealthLabel = new QLabel(this);//金钱标签
    QLabel* PlayerHealthIcon = new QLabel(this);//金钱图标

    int StateNum;//内部成员属性 记录关卡数
    int map_arr[8][12];//地图数组,注意与map_and_path.h中的宏定义保持一致  [********]
    int PlayerHealth = 10000;//玩家生命
    int Money = 100000;//玩家金钱资源


    MeleeTowerSelection melee_tower_selection;//【选择近战防御塔界面】
    RemoteTowerSelection remote_tower_selection;//【选择远程防御塔界面】

    //有关敌人的成员
    QVector<EnemyBase*> BirthVector;    //出生阶段的敌人
    QVector<EnemyBase*> OnStageVector;    //当前地图上存在的敌人:还有血量且没走到终点
    //有关防御塔的成员
    QVector<TowerBase*> MeleeTowerVector;//装目前场上存在的近战塔,



    //path_set_on_map : 基于地图的路径集 / 地图上的像素路径
    //相当于把原路径集path_set按照map_block_size比例放大后的路径集
    //直观上来说就是路径集对应地图上每一个砖块的左上角像素坐标
    QVector<QVector<location>> path_set_on_map;



    QVector<QString> splash;//存储怪物收到攻击时的拟声词特效图片路径的数组,构造函数中初始化,绘图时用

public:
    //explicit GameWindow(QWidget *parent = 0);

    //游戏界面的bgm
    QMediaPlayer *gamewindow_bgm = nullptr;

    //开始播放bgm
    void play_bgm(){ gamewindow_bgm->play();}

    //停止播放bgm
    void stop_bgm(){ gamewindow_bgm->stop();}

    //游戏页面的设置界面
    setting_scene * gamewindow_settings = nullptr;



    //成员函数：
    //*******************************************
    //构造函数与析构函数
    GameWindow(int num);
    ~GameWindow();

    bool cost_money(TowerBase*);//在防御塔上花钱的函数，假如买不起就return false

    //重要的功能性函数:
    //小工具函数：gamewindow类成员，判断鼠标是否点击了某个物件砖块，有则返回物件代号
    int CLICK_OBJECT(int mouseclick_x,int mouseclick_y);//判断鼠标是否点击了某个物件砖块,将鼠标点击区域和像素地图中的每个地砖对应，再对应到原始数组中判断这是墙、路、要塞还是可以放塔的地方，返回值为    0 墙壁   1 怪物路径   2 怪物出生点   3 防御塔位置   4 堡垒
    //小工具函数：gamewindow类成员,返回当前点击的格子的左上角坐标
    location CLICK_BLOCK(int mouseclick_x,int mouseclick_y);
    //小工具函数：判断鼠标点击区域是否和传入的塔所在的格子一致，本质就是算【鼠标点击处的像素坐标】和【塔所在的格的像素坐标范围】匹不匹配
    bool match_tower(int mouseclick_x,int mouseclick_y,TowerBase *);


    //绘图用函数
    //重写paintEvent事件 画游戏界面的所有东西
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent*); //鼠标点击事件重写
    //下面细分绘制不同东西的函数，由总函数paintEvent调用
    void paintMap(QPainter&);//绘制地图
    void paintEnemy4GuardingEffect(QPainter&);//绘制enemy_4法老守卫特效
    void paintEnemy(QPainter&);//绘制OnStageVector中的敌人
    void paintMeleeTowerSelection(QPainter&);//绘制【选择近战防御塔界面】
    void paintRemoteTowerSelection(QPainter&);//绘制【选择远程防御塔界面】
    void paintMeleeTowerUpdate(QPainter&);//绘制近战防御塔【升级拆除界面】
    void paintMeleeTower(QPainter&);//绘制近战防御塔

    void paintTowerAttackEffect_above(QPainter&);//绘制防御塔攻击特效子函数1：防御塔图片上层的特效
    void paintTowerAttackEffect_beneath(QPainter&);//绘制防御塔攻击特效子函数2：防御塔图片下层的特效

    void paintEnemyBeingAttackedEffect(QPainter&);//绘制敌人收到攻击时的拟声词特效

    void paintTowerBullet(QPainter&);//绘制子弹
    void paintHealthBar(QPainter&);//绘制血条



signals:
    //用于告诉选择关卡场景 点击了返回 的自定义信号
    void GameWindowBack();

public slots:

};

#endif // GAMEWINDOW_H

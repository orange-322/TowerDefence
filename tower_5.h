#ifndef TOWER_5_H
#define TOWER_5_H

//Tower_5:光明法师塔，远程塔，价值4000块,攻击范围为3个砖块，可以攻击空中和地面的所有敌人，受敌人攻击


#include"towerbase.h"
#include <QMediaPlayer>//多媒体模块下的音效头文件
class Tower_5:public TowerBase
{
public:
    Tower_5(location p,int BornTime);
    ~Tower_5();//析构函数
    QMediaPlayer *sound_effect = nullptr;//攻击声音特效
    void Attack(QVector<EnemyBase*>&,int current_GameTime) override;      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
    int last_attack_time=-1;//最近一次发出攻击的时间，用于使tower5一秒钟只发出一颗子弹

    ///注意tower5类只负责在敌人屁股后面放子弹，子弹打到敌人的攻击函数其实在EnemyBase::tower_bullet_move()里
};
#endif // TOWER_5_H

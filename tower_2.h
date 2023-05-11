#ifndef TOWER_2_H
#define TOWER_2_H

//Tower_2:箱子，近战塔，类似坚果墙，价值500块，受敌人攻击，攻击范围为0.9个砖块

#include"towerbase.h"

class Tower_2:public TowerBase
{
public:
    Tower_2(location p,int BornTime);
    ~Tower_2();//析构函数
    void Attack(QVector<EnemyBase*>&,int current_GameTime) override;      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
};

#endif // TOWER_2_H

#ifndef TOWER_1_H
#define TOWER_1_H


//Tower_1:地刺，近战塔，价值1000块，不受敌人攻击，攻击范围为半个砖块


#include"towerbase.h"

class Tower_1:public TowerBase
{
public:
    Tower_1(location p,int BornTime);//构造函数
    ~Tower_1();//析构函数
    void Attack(QVector<EnemyBase*>&,int current_GameTime) override;      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
};

#endif // TOWER_1_H

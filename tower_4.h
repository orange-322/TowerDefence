#ifndef TOWER_4_H
#define TOWER_4_H

//Tower_4:电磁塔，远程塔，价值3000块,攻击范围为1.7个砖块，可以攻击空中和地面的所有敌人，受敌人攻击


#include"towerbase.h"

class Tower_4:public TowerBase
{
public:
    Tower_4(location p,int BornTime);
    ~Tower_4();//析构函数
    void Attack(QVector<EnemyBase*>&,int current_GameTime) override;      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
};
#endif // TOWER_4_H

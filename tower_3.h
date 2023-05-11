#ifndef TOWER_3_H
#define TOWER_3_H

//Tower_3:地震塔，远程塔，价值2000块,攻击范围为1.7个砖块，只能攻击地面的敌人，受敌人攻击


#include"towerbase.h"

class Tower_3:public TowerBase
{
public:
    Tower_3(location p,int BornTime);
    ~Tower_3();//析构函数
    void Attack(QVector<EnemyBase*>&,int current_GameTime) override;      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
};
#endif // TOWER_3_H

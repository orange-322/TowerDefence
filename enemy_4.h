#ifndef ENEMY_4_H
#define ENEMY_4_H

#include"enemybase.h"

//第四类敌人:法老，地面敌人，不攻击防御塔，为友方回血
class Enemy_4:public EnemyBase
{
public:
    Enemy_4(QVector<location>);
    ~Enemy_4();
    virtual void Attack(QVector<TowerBase*>&,int current_GameTime) override;      //攻击函数,传入这个敌人范围内的塔vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
    virtual void Guard(QVector<EnemyBase*>&,int current_GameTime) override;      //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)

};

#endif // ENEMY_4_H

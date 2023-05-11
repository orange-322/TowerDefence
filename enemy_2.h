#ifndef ENEMY_2_H
#define ENEMY_2_H

#include"enemybase.h"

//第二类敌人:飞龙幼崽，飞行敌人，远距离攻击防御塔，畏战,攻击范围为2*map_block_size，为远程攻击类型的敌人
class Enemy_2:public EnemyBase
{
public:
    Enemy_2(QVector<location>);
    ~Enemy_2();
    virtual void Attack(QVector<TowerBase*>&,int current_GameTime) override;      //攻击函数,传入这个敌人范围内的塔vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
    virtual void Guard(QVector<EnemyBase*>&,int current_GameTime) override;      //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)

};

#endif // ENEMY_2_H

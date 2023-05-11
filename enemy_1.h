#ifndef ENEMY_1_H
#define ENEMY_1_H

#include"enemybase.h"

//第一类敌人:野猪，地面敌人，近距离攻击防御塔,攻击范围为0.91*map_block_size(稍稍大于tower2箱子的攻击范围)
class Enemy_1:public EnemyBase
{
public:
    Enemy_1(QVector<location>);
    ~Enemy_1();
    virtual void Attack(QVector<TowerBase*>&,int current_GameTime) override;      //攻击函数,传入这个敌人范围内的塔vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
    virtual void Guard(QVector<EnemyBase*>&,int current_GameTime) override;      //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)

};

#endif // ENEMY_1_H

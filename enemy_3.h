#ifndef ENEMY_3_H
#define ENEMY_3_H

#include"enemybase.h"

//第三类敌人:外星飞碟，飞行敌人，远距离攻击防御塔，恋战,攻击范围为2*map_block_size，为远程攻击类型的敌人
class Enemy_3:public EnemyBase
{
public:
    Enemy_3(QVector<location>);
    ~Enemy_3();
    virtual void Attack(QVector<TowerBase*>&,int current_GameTime) override;      //攻击函数,传入这个敌人范围内的塔vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
    virtual void Guard(QVector<EnemyBase*>&,int current_GameTime) override;      //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)

};

#endif // ENEMY_3_H

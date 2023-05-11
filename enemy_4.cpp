#include "enemy_4.h"

#include"towerbase.h"

//第四类敌人:法老，地面敌人，不攻击防御塔，为友方回血

Enemy_4::Enemy_4(QVector<location> p):EnemyBase(p)
{
    type = 4;//类型为4
    flying_enemy = false;//设置敌人是飞行敌人还是地面敌人
    speed = 10;original_speed = speed;//设置敌人移速
    power = 0;//这个敌人的攻击威力
    attack_range = 0;//这个敌人在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    width = 60;//宽度设定成和砖块大小一样
    height = 60;//基于设定的飞龙幼崽图片的比例设定绘制时的比例
    full_hp = 100;
    current_hp = full_hp;
    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/enemy_4.png");

}
Enemy_4::~Enemy_4()
{/**/}
void Enemy_4::Enemy_4::Attack(QVector<TowerBase*>& tower_in_range,int current_GameTime)       //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{/**/}


void Enemy_4::Enemy_4::Guard(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)     //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)
{
    if(current_GameTime%4 == 2)//每过4秒 就对范围内友方敌人发起一次治疗
    {
        if(!enemy_in_range.isEmpty())//攻击范围内有敌人的话
        {
            is_guarding = true;//切换为正在守卫的状态
            for(auto enemy_item = enemy_in_range.begin();enemy_item!=enemy_in_range.end();enemy_item++)
            {
                //test
                QString test_enemy4 = QString("发动防御,法老范围内有%1个友方单位").arg(enemy_in_range.size());
                qDebug()<<test_enemy4;

                (*enemy_item)->set_hp((*enemy_item)->get_current_hp() + 10);//每次加10血
                (*enemy_item)->is_healing = true;

            }
        }
        else
            is_guarding = false;//攻击范围内没有敌人，维持is_attacking为false的状态

    }
    else
    {
        is_guarding = false;//在攻击间歇，置攻击状态为false
    }

}

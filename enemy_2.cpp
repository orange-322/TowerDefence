#include "enemy_2.h"

#include"towerbase.h"

//第二类敌人:飞龙幼崽，飞行敌人，远距离攻击防御塔，畏战,攻击范围为2*map_block_size，为远程攻击类型的敌人


Enemy_2::Enemy_2(QVector<location> p):EnemyBase(p)
{
    type = 2;//类型为2
    flying_enemy = true;//设置敌人是飞行敌人还是地面敌人
    speed = 10;original_speed = speed;//设置敌人移速
    power = 1;//这个敌人的攻击威力
    attack_range = map_block_size*2;//这个敌人在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    width = 60;//宽度设定成和砖块大小一样
    height = 60;//基于设定的飞龙幼崽图片的比例设定绘制时的比例
    full_hp = 90;
    current_hp = full_hp;
    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/enemy_2.png");

}
Enemy_2::~Enemy_2()
{/**/}
void Enemy_2::Enemy_2::Attack(QVector<TowerBase*>& tower_in_range,int current_GameTime)       //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{
    //怪对塔的攻击
    //test
    //qDebug()<<"飞龙宝宝检测到敌人";
    //if(current_GameTime%(attack_interval+1) == attack_interval)//如果 每过attack_interval秒 就对范围内防御塔发起一次攻击
    //{

        for(auto tower_item = tower_in_range.begin();tower_item!=tower_in_range.end();tower_item++)
        {
//            //test
//            QString e2 = QString("第%1秒：飞龙宝宝攻击成功了一个防御塔一次").arg(current_GameTime);
//            qDebug()<<e2;

            (*tower_item)->set_hp((*tower_item)->get_current_hp() - power);


        }

    //}
}

void Enemy_2::Enemy_2::Guard(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)     //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)
{/**/}

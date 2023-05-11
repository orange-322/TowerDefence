#include "enemy_3.h"

#include"towerbase.h"

//第三类敌人:外星飞碟，飞行敌人，远距离攻击防御塔，恋战,攻击范围为2*map_block_size，为远程攻击类型的敌人

Enemy_3::Enemy_3(QVector<location> p):EnemyBase(p)
{
    type = 3;//类型为3
    flying_enemy = true;//设置敌人是飞行敌人还是地面敌人
    speed = 10;original_speed = speed;//设置敌人移速
    power = 1.5;//这个敌人的攻击威力
    attack_range = map_block_size*1.4;//这个敌人在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    width = 60;//宽度设定成和砖块大小一样
    height = 60;//基于设定的飞龙幼崽图片的比例设定绘制时的比例
    full_hp = 200;
    current_hp = full_hp;
    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/enemy_3.png");

}
Enemy_3::~Enemy_3()
{/**/}
void Enemy_3::Enemy_3::Attack(QVector<TowerBase*>& tower_in_range,int current_GameTime)       //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{
    //怪对塔的攻击
    //test
    //qDebug()<<"外星飞碟检测到敌人";

    if(tower_in_range.isEmpty())//攻击范围内不存在防御塔，设置正常速度
    {
        set_speed(get_original_speed());
    }
    else//攻击范围内有防御塔，将速度设置为0，对防御塔攻击
    {

    //1.将速度设置为0
    set_speed(0);

    //2.攻击防御塔
    //if(current_GameTime%(attack_interval+1) == attack_interval)//如果 每过attack_interval秒 就对范围内防御塔发起一次攻击
    //{

        for(auto tower_item = tower_in_range.begin();tower_item!=tower_in_range.end();tower_item++)
        {
//            //test
//            QString e2 = QString("第%1秒：外星飞碟攻击成功了一个防御塔一次").arg(current_GameTime);
//            qDebug()<<e2;

            (*tower_item)->set_hp((*tower_item)->get_current_hp() - power);


        }

    //}
    }
}


void Enemy_3::Enemy_3::Guard(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)     //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)
{/**/}

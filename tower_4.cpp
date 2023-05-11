//Tower_4:电磁塔，远程塔，价值3000块,攻击范围为1.7个砖块，可以攻击空中和地面的所有敌人，受敌人攻击

#include "tower_4.h"

Tower_4::Tower_4(location p,int BornTime):TowerBase(p,BornTime)
{
    type = 4;//类型为4
    value = 3000;//这个塔价值3000块
    attack_range = map_block_size*1.7;//这个塔在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    power = 15;//这个塔的攻击威力
    attack_interval = 2;//这个塔每隔2秒发一次攻击
    full_hp = 150;
    current_hp = full_hp;//这个塔当前的血量，当前设置为了演示外星飞碟停下来把电磁塔打爆再移动而设成80
    width = map_block_size;
    height = map_block_size;

    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/tower_4.png");


}
Tower_4::~Tower_4()//析构函数
{/**/}
void Tower_4::Attack(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{

    if((current_GameTime-get_borntime())%(attack_interval) == 1)//放置该塔后过1秒才开始攻击 每过attack_interval秒 就对范围内敌人发起一次攻击
    {
        if(!enemy_in_range.isEmpty())//攻击范围内有敌人的话
        {
            is_attacking = true;//切换为正在攻击的状态
            for(auto enemy_item = enemy_in_range.begin();enemy_item!=enemy_in_range.end();enemy_item++)
            {
//                //test
//                QString t4 = QString("第%1秒：电磁塔攻击成功了一个敌人一次").arg(current_GameTime);
//                qDebug()<<t4;
                (*enemy_item)->set_hp((*enemy_item)->get_current_hp() - power);
                (*enemy_item)->is_being_attacked = 1;//对应第1类攻击特效(蓝色的那个splash2.png)
            }
        }
        else
            is_attacking = false;//攻击范围内没有敌人，维持is_attacking为false的状态

    }
    else
    {
        is_attacking = false;//在攻击间歇，置攻击状态为false
    }
}

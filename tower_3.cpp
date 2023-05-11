//Tower_3:地震塔，远程塔，价值2000块,攻击范围为1.7个砖块，受敌人攻击

#include "tower_3.h"

Tower_3::Tower_3(location p,int BornTime):TowerBase(p,BornTime)
{
    type = 3;//类型为3
    value = 2000;//这个塔价值2000块
    attack_range = map_block_size*1.7;//这个塔在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    power = 20;//这个塔的攻击威力
    attack_interval = 4;//这个塔每隔4秒发一次攻击
    full_hp = 1000000;
    current_hp = full_hp;//这个塔当前的血量,地震塔设置成无限血量
    width = map_block_size;
    height = map_block_size;

    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/tower_3_1.png");


}
Tower_3::~Tower_3()//析构函数
{/**/}
void Tower_3::Attack(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{
    /// phase2观点:地震塔不受敌人攻击,在其他类型的塔中加入受敌人攻击的内容

    if((current_GameTime-get_borntime())%(attack_interval) == 2)//放置该塔后过2秒才开始攻击 每过attack_interval秒 就对范围内敌人发起一次攻击
    {
        is_attacking = true;//攻击时间，设置攻击状态为true
        for(auto enemy_item = enemy_in_range.begin();enemy_item!=enemy_in_range.end();enemy_item++)
        {

            if(!(*enemy_item)->is_flying_enemy())//如果敌人是地面敌人，不是飞行敌人，tower3地震塔才能攻击
            {
                //test
                QString t3 = QString("第%1秒：地震塔攻击成功了一个敌人一次").arg(current_GameTime);
                qDebug()<<t3;
                (*enemy_item)->set_hp((*enemy_item)->get_current_hp() - power);
                (*enemy_item)->is_being_attacked = 4;//对应第4类攻击特效(红色的那个splash5.png)
            }
        }

    }
    else
    {
        is_attacking = false;//在攻击间歇，置攻击状态为false
    }
}

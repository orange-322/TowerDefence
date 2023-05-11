
//Tower_1:地刺，近战塔，价值1000块，不受敌人攻击，攻击范围为半个砖块

#include "tower_1.h"


Tower_1::Tower_1(location p,int BornTime):TowerBase(p,BornTime)
{
    type = 1;//类型为1
    value = 1000;//这个塔价值1000块
    attack_range = map_block_size/2;//这个塔在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    power = 10;//这个塔的攻击威力
    attack_interval = 1;//这个塔每隔一秒发一次攻击
    full_hp = 1000000;
    current_hp = full_hp;//这个塔当前的血量,地刺塔设置成无限血量
    width = map_block_size;
    height = map_block_size;

    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/tower_1.png");


}
Tower_1::~Tower_1()//析构函数
{/**/}
void Tower_1::Attack(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{
    //敌人对防御塔的攻击由于include的问题tower的参传不过去，故防御塔遭受敌人攻击暂时写在这里
    /////////////////////////////////////////////////////////////////////////////////////[########]
    ///TODO 待写
    /// phase2观点:地刺塔不受敌人攻击,在其他类型的塔中加入受敌人攻击的内容

    //if(current_GameTime%(attack_interval+1) == attack_interval)//如果 每过attack_interval秒 就对范围内敌人发起一次攻击
    //{

        for(auto enemy_item = enemy_in_range.begin();enemy_item!=enemy_in_range.end();enemy_item++)
        {

            if(!(*enemy_item)->is_flying_enemy())//如果敌人是地面敌人，不是飞行敌人，tower1地刺才能攻击
            {
                //test
                QString t1 = QString("第%1秒：地刺攻击成功了一个敌人一次").arg(current_GameTime);
                qDebug()<<t1;
                is_attacking = true;//切换为正在攻击的状态
                (*enemy_item)->set_hp((*enemy_item)->get_current_hp() - power);//扣除敌人血量
                (*enemy_item)->is_being_attacked = 0;//对应第0类攻击特效(紫色的那个splash1.png)
            }
        }

    //}
}

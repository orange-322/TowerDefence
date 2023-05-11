#include "tower_2.h"

//Tower_2:箱子，近战塔，类似坚果墙，价值500块，受敌人攻击，攻击范围为0.9个砖块


Tower_2::Tower_2(location p,int BornTime):TowerBase(p,BornTime)
{
    type = 2;//类型为2
    value = 500;//这个塔价值500块
    attack_range = map_block_size*0.9;//这个塔在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    power = 0;//这个塔的攻击威力
    attack_interval = 1;//这个塔每隔一秒发一次攻击
    full_hp = 70;
    current_hp = full_hp;//这个塔当前的血量
    width = map_block_size;
    height = map_block_size;

    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/tower_2.png");


}
Tower_2::~Tower_2()//析构函数
{/**/}
void Tower_2::Attack(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{
    //敌人对防御塔的攻击由于include的问题tower的参传不过去，故防御塔遭受敌人攻击暂时写在这里
    for(auto enemy_item = enemy_in_range.begin();enemy_item!=enemy_in_range.end();enemy_item++)
    {
        ///下面这句成功移植到了enemy类中的attack函数
        //set_hp(get_current_hp()-(*enemy_item)->get_power());//模拟tower2遭受敌人的攻击
        if(!(*enemy_item)->is_flying_enemy())//如果敌人是地面敌人，不是飞行敌人，tower2才能阻拦
        {
            is_attacking = true;//切换为正在攻击的状态
            (*enemy_item)->set_speed(0);//箱子把敌人速度降为0
            (*enemy_item)->is_being_attacked = 3;//对应第3类攻击特效(黄色的那个splash4.png)
            if(get_current_hp()<=10){
                //test
    //            QString t = QString("恢复敌人初始速度:%1").arg((*enemy_item)->get_original_speed());
    //            qDebug()<<t;
                (*enemy_item)->set_speed( (*enemy_item)->get_original_speed() );//检查如果箱子没血了，就把敌人的速度恢复原状
            }
        }

    }


//    //if(current_GameTime%(attack_interval+1) == attack_interval)//如果 每过attack_interval秒 就对范围内敌人发起一次攻击
//    //{
//        for(auto enemy_item = enemy_in_range.begin();enemy_item!=enemy_in_range.end();enemy_item++)
//        {
//            qDebug()<<"箱子攻击成功了一个敌人一次（虽然没有攻击力）";
//            (*enemy_item)->set_hp((*enemy_item)->get_current_hp() - power);
//        }
//    //}
}

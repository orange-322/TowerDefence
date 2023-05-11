//Tower_5:光明法师塔，发射子弹的远程塔，价值4000块,攻击范围为3个砖块，可以攻击空中和地面的所有敌人，受敌人攻击
///注意tower5类只负责在敌人屁股后面放子弹，子弹打到敌人的攻击函数其实在EnemyBase::tower_bullet_move()里

#include "tower_5.h"

Tower_5::Tower_5(location p,int BornTime):TowerBase(p,BornTime)
{
    type = 5;//类型为5
    value = 4000;//这个塔价值4000块
    attack_range = map_block_size*3;//这个塔在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    power = 0;//这个塔的攻击威力（
    attack_interval = 3;//这个塔每隔3秒发一次攻击
    full_hp = 300;
    current_hp = full_hp;//这个塔当前的血量，当前设置为了演示外星飞碟停下来把电磁塔打爆再移动而设成80
    width = map_block_size;
    height = map_block_size;

    //设置它的图片
    picture_path->clear();
    picture_path->append("://res/tower_5.png");

    sound_effect = new QMediaPlayer;
    sound_effect->setMedia(QUrl::fromLocalFile("D:/Qt/my_own/chap02/TowerDefence/music/tower_5_sound_effect.mp3"));
    sound_effect->setVolume(65);
}
Tower_5::~Tower_5()//析构函数
{
    delete sound_effect;
}
void Tower_5::Attack(QVector<EnemyBase*>& enemy_in_range,int current_GameTime)      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
{
    if(last_attack_time == current_GameTime) {return;}//如果在同一秒内发出过子弹，就直接结束函数不再攻击
    else last_attack_time = current_GameTime;//否则继续下面的攻击流程，并更新最近一次攻击时间为当前时间

    if((current_GameTime-get_borntime())%(attack_interval) == 1)//放置该塔后过1秒才开始攻击 每过attack_interval秒 就对范围内敌人发起一次攻击
    {
        if(!enemy_in_range.isEmpty())//攻击范围内有敌人的话
        {
            is_attacking = true;//切换为正在攻击的状态
//            for(auto enemy_item = enemy_in_range.begin();enemy_item!=enemy_in_range.end();enemy_item++)
//            {
                auto enemy_item = enemy_in_range.begin();//只攻击范围内第一个敌人，去掉了for+break的形式
                //test
                QString t5 = QString("第%1秒：光明法师塔攻击成功了一个敌人一次").arg(current_GameTime);
                qDebug()<<t5;

                //对敌人的攻击
                location p;
                p.x = get_position().x+15;
                p.y = get_position().y;//子弹最开始的位置(设定在光明塔正上方居中一点的地方)

                TowerBullet *new_bullet = new TowerBullet(p);
                (*enemy_item)->add_tower_bullet(new_bullet);//在这个敌人屁股后放上了一个追击子弹！让子弹飞一会儿！
                sound_effect->play();//播放攻击特效音
//                break;
//            }
        }
        else
            is_attacking = false;//攻击范围内没有敌人，维持is_attacking为false的状态

    }
    else
    {
        is_attacking = false;//在攻击间歇，置攻击状态为false
    }
}

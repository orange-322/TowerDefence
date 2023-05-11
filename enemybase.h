#ifndef ENEMYBASE_H
#define ENEMYBASE_H

//可能出现头文件重复引用问题   [********]
#include"towerbullet.h"


///[********]神来之笔，解决了enemybase类和towerbase类中声明的attack函数相互包含对方类类型参数的问题。（类似于相互include）
class TowerBase;//前置声明TowerBase类


class EnemyBase
{
protected://为了方便派生类使用，使用protected而不是private

    int type;//敌人类型，对应文件enemy_x.cpp中数字x
    bool flying_enemy;//当前敌人是否是飞行敌人(和type变量的功能有重叠)


    location position;//这个敌人在当前这一帧的位置（地图上的像素坐标）
    QVector<location> path;//这个敌人他在地图上行走的路径

    int original_speed = 10;//敌人原本的speed，初值与speed相同，服务于speed的改变与恢复
    int speed = 10;//这个敌人在地图上移动的速度(以像素为单位)  (注意必须是map_block_size的公因数，否则无法判断到达了终点    [********])

    double power;//这个敌人的攻击威力
    double attack_range;//这个敌人在地图上的攻击范围(以像素为单位，和绘制的地图匹配)

    double full_hp;//这个敌人的满血,用于绘制血条
    double current_hp;//这个敌人当前的血量

    QString* picture_path = new QString;//绘图时用，这个敌人的图片路径
    int width,height;//绘图时用，这个敌人图片的宽高



public:
    int is_being_attacked = -1;//敌人收到攻击时的状态，-1表示没受到攻击状态，0~5表示收到攻击的不同类型，直接对应splash图片路径数组每个图片路径元素的下标
    bool is_guarding = false;//是否正在守卫（为队友加BUFF）状态，用于绘制守卫特效
    bool is_healing = false;//是否正在回血状态，用于绘制回血特效

    EnemyBase(QVector<location>&);//构造函数
    virtual ~EnemyBase();//析构函数
    int get_type()const{return type;}//返回敌人类型
    location get_position()const{return position;}
    QString get_picture_path()const{return (*picture_path);}
    int get_width()const{return width;}
    int get_height()const{return height;}

    void set_hp(double new_hp){
        if(new_hp > full_hp)
            current_hp = full_hp;
        else
            current_hp = new_hp;
    }//设置血量，防御塔攻击用
    double get_full_hp()const{return full_hp;}//返回满血血量，用于绘制血条用
    double get_current_hp()const{return current_hp;}//返回现在这个敌人的血量

    void set_speed(int new_speed){speed = new_speed;}//设置移动速度
    int get_speed()const{return speed;}
    int get_original_speed()const{return original_speed;}//返回原本的移速

    double get_power()const{return power;}//返回敌人攻击力

    bool is_flying_enemy()const{return flying_enemy;}//返回这个敌人是否是飞行敌人,true就是飞行敌人,false就是地面敌人

    bool move();//敌人的移动函数，如果没移动到终点就返回true(还能移动)，移动到终点了就返回false

    double get_attack_range()const{return attack_range;}//返回塔的攻击范围
    virtual void Attack(QVector<TowerBase*>&,int current_GameTime)=0;      //攻击函数,传入这个敌人范围内的塔vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
    virtual void Guard(QVector<EnemyBase*>&,int current_GameTime)=0;      //守卫函数,与攻击函数相对,传入这个敌人范围内的敌人vector,还有当前这一帧的游戏时间(和守卫间隔相配合)


    QVector<TowerBullet*> tower_bullet_vec;//这个敌人屁股后的子弹,为了绘图方便放在设置为public变量
    void add_tower_bullet(TowerBullet* b){tower_bullet_vec.append(b);}//向vec中添加子弹，即把一枚导弹锁定这个敌人并发射
    bool tower_bullet_move();//追击这个敌人的子弹的移动,包含了子弹击中敌人后的消除。

};

#endif // ENEMYBASE_H

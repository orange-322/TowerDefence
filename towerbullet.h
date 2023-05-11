#ifndef TOWERBULLET_H
#define TOWERBULLET_H

#include"global_structure.h"

class TowerBullet
{
private:
    location position;//这颗子弹在当前这一帧的位置（地图上的像素坐标）
    int speed = 1;//这颗子弹在地图上移动的速度(以像素为单位)  (注意必须是map_block_size的公因数，否则无法判断到达了终点    [********])
    double power;//这颗子弹的攻击威力
public:
    TowerBullet(location p);
    location get_position()const{return position;}
    int x()const{return position.x;}
    int y()const{return position.y;}

    int get_speed()const{return speed;}
    double get_power()const{return power;}//返回子弹攻击力

//    void move();//子弹的移动函数,考虑后决定写在enemybase里
    void add_x(){position.x+=speed;}
    void add_y(){position.y+=speed;}
    void decrease_x(){position.x-=speed;}
    void decrease_y(){position.y-=speed;}//移动函数，辅助EnemyBase里的tower_bullet_move函数


};

#endif // TOWERBULLET_H

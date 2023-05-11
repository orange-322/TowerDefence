#ifndef TOWERBASE_H
#define TOWERBASE_H

#include"enemybase.h"


class TowerBase
{
protected:
    int type;//防御塔类型，对应文件tower_x.cpp中数字x
    int borntime;//防御塔最初放置的时间，用于让其间歇攻击
    location position;//防御塔的位置(统一规定成绘图时的像素坐标，【放大】+【加上高的基数】的那种)
    double full_hp;//这个塔的满血，用于绘制血条
    double current_hp;//这个塔当前的血量
    int value;//这个塔的价值(开销)
    double attack_range;//这个塔在地图上的攻击范围(以像素为单位，和绘制的地图匹配)
    int attack_interval;             //攻击时间间隔，单位为秒，和gamewindow中的GameTime匹配
    double power;//这个塔的攻击威力


    QString* picture_path = new QString;//绘图时用，这个塔的图片路径
    int width,height;//绘图时用，这个塔图片的宽高
    bool update_remove_show = false;//标志位是否显示【升级拆除界面】,是towerbase类的成员


public:
    bool is_attacking = false;//是否正在攻击状态，用于绘制攻击特效

    int get_type()const{return type;}//返回防御塔类型
    location get_position()const{return position;}//返回当前塔所在的坐标
    int get_borntime()const{return borntime;}//返回当前塔最开始放置的时间
    void set_hp(double new_hp){//设置血量，怪物攻击用
        if(new_hp > full_hp)
            current_hp = full_hp;
        else
            current_hp = new_hp;
    }
    double get_full_hp()const{return full_hp;}//返回当前塔的满血，用于绘制血条
    double get_current_hp()const{return current_hp;}//返回当前塔的hp
    double get_width()const{return width;}//返回塔图片的宽
    double get_height()const{return height;}//返回塔图片的高
    int get_value()const{return value;}//返回塔的价值
    double get_attack_range()const{return attack_range;}//返回塔的攻击范围
    QString get_picture_path()const{return *picture_path;}//返回这个塔的图片的路径

    bool update_remove_is_showed(){return update_remove_show;}//这个塔的【升级拆除界面】是否在显示
    void do_show_update_remote(){update_remove_show = true;}//将这个塔的【升级拆除界面】设置为显示
    void hide_update_remote(){update_remove_show = false;}//将这个塔的【升级拆除界面】设置为不显示


    TowerBase(location p,int BornTime);//构造函数
    virtual ~TowerBase();//析构函数
    virtual void Attack(QVector<EnemyBase*>&,int current_GameTime)=0;      //攻击函数,传入这个塔范围内的敌人vector,还有当前这一帧的游戏时间(和攻击间隔相配合)
};

#endif // TOWERBASE_H

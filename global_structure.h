#ifndef GLOBAL_STRUCTURE_H
#define GLOBAL_STRUCTURE_H

#include<QVector>
#include<QString>
#include<QDebug>
#include<QtMath>

//定义全局结构体（自定义变量）


//地图数组中 0为墙 1为路径 2为怪物出生点 3为防御塔可放置的位置 4为堡垒要塞
//其中BIRTH 和 PATH的位置都可以放近战攻击单位
#define WALL 0
#define PATH 1
#define BIRTH 2
#define TOWER 3
#define FORTRESS 4



//存放 堡垒、路、塔、墙 的资源图路径
#define FORTRESS_PIC_PATH "://res/map/fortress.png"
#define PATH_PIC_PATH "://res/map/path.png"
#define TOWER_PIC_PATH "://res/map/tower.png"
#define WALL_PIC_PATH "://res/map/wall.png"





//固定每张地图的行数 列数
//这里要与gamewindow.h中定义的成员数组map_arr的大小保持一致       [********]
#define MAP_ROW 8
#define MAP_COL 12




//绘图用:每个地砖块的绘图尺寸
constexpr int map_block_size = 60;//设置编译期常量，在编译时优化，放在静态数据区，可以解决重复定义的问题

//为了空出菜单栏，起始绘图要往下一点开始绘制，这个就是起始的高度(实际的菜单栏高度好像只有20多，不过留白一点无所谓)
constexpr int start_height_pixel = 55;//设置编译期常量，在编译时优化，放在静态数据区，可以解决重复定义的问题
//这个项目中所有的像素坐标都是【原始数组坐标】【按map_block_size比例放大】【高度加上start_height_pixel】后的结果    判断鼠标点击区域时尤其注意这点    [********]

//单位时间增加的金币资源
constexpr int MONEY_INCREMENT = 1;//设置编译期常量，在编译时优化，放在静态数据区，可以解决重复定义的问题

//生成怪物的时间间隔(ms)
constexpr int GENERATE_ENEMY_INTERVAL = 1000;//设置编译期常量，在编译时优化，放在静态数据区，可以解决重复定义的问题



//路径中每一个点
struct location{
    int x;
    int y;
    location(const int a = 0,const int b = 0):x(a),y(b){}//()里加不加const??     [********]
};

























#endif // GLOBAL_STRUCTURE_H

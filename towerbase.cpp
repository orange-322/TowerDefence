#include "towerbase.h"


TowerBase::TowerBase(location p,int BornTime)
{
    //把在地图上的坐标位置填好
    position.x = p.x;
    position.y = p.y;
    borntime = BornTime;

}

TowerBase::~TowerBase()//析构函数
{
    delete picture_path;
}

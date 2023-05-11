#include "enemybase.h"

EnemyBase::EnemyBase(QVector<location>& p)
{
    this->path = p;
    //取出成员变量路径path中第一个点，将其初始化并存入birth数组中
    position.x = path.at(0).x;
    position.y = path.at(0).y;
    path.remove(0);
}
EnemyBase::~EnemyBase()//析构函数
{
    delete picture_path;

    for(auto it = tower_bullet_vec.begin();it!=tower_bullet_vec.end();)
    {
        TowerBullet* del = (*it);
        tower_bullet_vec.erase(it);
        delete del;
    }
}

bool EnemyBase::move()//敌人的移动函数，即依据path,在单帧内改变其position的值即可
{
    if(path.isEmpty())
        return false;
    if(position.x>path.at(0).x){
        position.x-=speed;
        return true;
    }
    if(position.y>path.at(0).y){
        position.y-=speed;
        return true;
    }
    if(position.x<path.at(0).x){
        position.x+=speed;
        return true;
    }
    if(position.y<path.at(0).y){
        position.y+=speed;
        return true;
    }
    //若移动到了下一个点就把走过的上一个点删去
    if((position.y == path.at(0).y) && (position.x == path.at(0).x)){// 这里还可以再做优化，与这个顶点垂直且距离小于n pixel时就可以拐弯了，这样可以实现曲线拐弯，且不影响走直线     [########]
        path.remove(0);
    }
    return true;

}


bool EnemyBase::tower_bullet_move()//追击这个敌人的子弹的移动,包含了子弹击中敌人后的消除。
{
    int target_x = get_position().x;
    int target_y = get_position().y;//当前怪的坐标
    //版本1：使用迭代器，出现了堆溢出的情况，还不确定是否是这里的问题，也有可能是析构函数全部没写的问题
    for(auto tower_bullet_item = tower_bullet_vec.begin();tower_bullet_item!=tower_bullet_vec.end();)//注意这里不能直接++，因为涉及erase/remove操作
    {
        int bullet_x = (*tower_bullet_item)->x();
        int bullet_y = (*tower_bullet_item)->y();//当前子弹的坐标

        //检查子弹有没有"击中"敌人(与敌人的距离小于特定值)
        if((target_x-bullet_x)*(target_x-bullet_x)+
                (target_y-bullet_y)*(target_y-bullet_y)
                < 15*15)///直线距离小于15px
        {//子弹击中了敌人
            //敌人扣血
            set_hp(get_current_hp()-(*tower_bullet_item)->get_power());//记得在外(gamewindow调用这里的定时器)检查敌人血量是否减到0
            is_being_attacked = 5;//对应第5类攻击特效(荧光黄色的那个splash6.png)
            //子弹移除
            TowerBullet* del = (*tower_bullet_item);
            tower_bullet_vec.erase(tower_bullet_item);
            delete del;//释放空间
        }
        else
        {//子弹还没击中敌人
            //移动一下子弹
            if(target_x > bullet_x)
                (*tower_bullet_item)->add_x();
            else if(target_x < bullet_x)
                (*tower_bullet_item)->decrease_x();
            if(target_y > bullet_y)
                (*tower_bullet_item)->add_y();
            else if(target_y < bullet_y)
                (*tower_bullet_item)->decrease_y();//子弹朝着敌人移动一下
            //循环迭代器自增
            tower_bullet_item++;
        }
    }
//    //版本2：不用迭代器（起因是出现了堆溢出的现象,想看看是不是迭代器的关系，后面发现不是，故重新采用版本1
//    for(int i = 0;i<tower_bullet_vec.size();i++)
//    {
//        TowerBullet* tower_bullet_item = tower_bullet_vec[i];
//        int bullet_x = tower_bullet_item->x();
//        int bullet_y = tower_bullet_item->y();//当前子弹的坐标

//        //检查子弹有没有"击中"敌人(与敌人的距离小于特定值)
//        if((target_x-bullet_x)*(target_x-bullet_x)+
//                (target_y-bullet_y)*(target_y-bullet_y)
//                < 15*15)///直线距离小于15px
//        {//子弹击中了敌人
//            //敌人扣血
//            set_hp(get_current_hp()-tower_bullet_item->get_power());//记得在外(gamewindow调用这里的定时器)检查敌人血量是否减到0
//            //子弹移除
//            TowerBullet* del = tower_bullet_item;
//            tower_bullet_vec.erase(tower_bullet_vec.begin()+i);
//            delete del;//释放空间[********]
//        }
//        else
//        {//子弹还没击中敌人
//            //移动一下子弹
//            if(target_x > bullet_x)
//                tower_bullet_item->add_x();
//            else if(target_x < bullet_x)
//                tower_bullet_item->decrease_x();
//            if(target_y > bullet_y)
//                tower_bullet_item->add_y();
//            else if(target_y < bullet_y)
//                tower_bullet_item->decrease_y();//子弹朝着敌人移动一下
//            //循环迭代器自增
//            i++;
//        }
//    }
    return true;
}

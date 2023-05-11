#ifndef MAP_AND_PATH_H
#define MAP_AND_PATH_H

#include<QDebug>


#include"global_structure.h"


//全局头文件map_and_path:






//存放地图:
//1-9关对应的地图下标分别为0-8
int all_state_map[9][MAP_ROW][MAP_COL]=
{
    //第0张地图
    {
        1,1,1,1,1,1,1,0,0,0,0,2,
        1,1,1,1,1,1,1,3,0,0,0,1,
        1,1,3,0,0,0,0,3,0,0,3,1,
        1,1,1,1,3,1,1,1,1,1,1,1,
        1,3,3,1,1,1,1,1,3,0,0,0,
        1,3,3,1,1,1,1,1,3,0,1,2,
        4,1,1,1,1,3,3,1,1,1,1,0,
        4,1,1,1,1,1,1,1,1,1,1,2
    },
    //第1张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    },
    //第2张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    },
    //第3张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    },
    //第4张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    },
    //第5张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    },
    //第6张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    },
    //第7张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    },
    //第8张地图
    {
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0
    }

};




//下面开始搞路径：
//"声明路径" "为路径赋值" "将路径添加到路径集"    每更改一次path，这三个注释处都要同步更改      [********]


////////////////////////////////////////test把这个改成结构体“构造”函数，直接在global_structure.h的结构体中定义
//构造一个location结构体
//location p(int x,int y)
//{
//    location ans;
//    ans.x = x;
//    ans.y = y;
//    return ans;
//}


//地图路径类：存取一个关卡的路径集
class Paths
{
private:

    //当前关卡数
    int state_num;


    //为方便存取点坐标，先把每个点封装起来[0-7][0-11],loc中每个元素就是其所在的下标
    location loc[8][12];


    //存放该关卡所有路径的路径集
    QVector<QVector<location>> path_set;

    //声明路径：一个QVector存一条路径
    QVector<location> path1_1;//关卡1的第1条路径
    QVector<location> path1_2;//关卡1的第2条路径
    QVector<location> path1_3;//关卡1的第3条路径
    //*****每条路径都是写死的********

public:
    //构造函数:将某关卡所有路径存入该关卡路径集
    Paths(int num)
    {
        //先初始化loc[][]数组
        for(int i=0;i<8;i++)
            for(int j=0;j<12;j++)
                loc[i][j] = location(i,j);


        //为路径赋值（固定值）
        //*******************************写死路径***************************************************************************
        //path1_1
        path1_1.append(loc[0][11]);
        path1_1.append(loc[1][11]);
        path1_1.append(loc[2][11]);
        path1_1.append(loc[3][11]);
        path1_1.append(loc[3][10]);
        path1_1.append(loc[3][9]);
        path1_1.append(loc[3][8]);
        path1_1.append(loc[3][7]);
        path1_1.append(loc[4][7]);
        path1_1.append(loc[5][7]);
        path1_1.append(loc[5][6]);
        path1_1.append(loc[4][6]);
        path1_1.append(loc[3][6]);
        path1_1.append(loc[3][5]);
        path1_1.append(loc[4][5]);
        path1_1.append(loc[5][5]);
        path1_1.append(loc[5][4]);
        path1_1.append(loc[5][3]);
        path1_1.append(loc[4][3]);
        path1_1.append(loc[3][3]);
        path1_1.append(loc[3][2]);
        path1_1.append(loc[3][1]);
        path1_1.append(loc[2][1]);
        path1_1.append(loc[1][1]);
        path1_1.append(loc[1][2]);
        path1_1.append(loc[1][3]);
        path1_1.append(loc[1][4]);
        path1_1.append(loc[1][5]);
        path1_1.append(loc[1][6]);
        path1_1.append(loc[0][6]);
        path1_1.append(loc[0][5]);
        path1_1.append(loc[0][4]);
        path1_1.append(loc[0][3]);
        path1_1.append(loc[0][2]);
        path1_1.append(loc[0][1]);
        path1_1.append(loc[0][0]);
        path1_1.append(loc[1][0]);
        path1_1.append(loc[2][0]);
        path1_1.append(loc[3][0]);
        path1_1.append(loc[4][0]);
        path1_1.append(loc[5][0]);
        path1_1.append(loc[6][0]);


        //path1_2
        path1_2.append(loc[5][11]);
        path1_2.append(loc[5][10]);
        path1_2.append(loc[6][10]);
        path1_2.append(loc[6][9]);
        path1_2.append(loc[6][8]);
        path1_2.append(loc[6][7]);
        path1_2.append(loc[5][7]);
        path1_2.append(loc[4][7]);
        path1_2.append(loc[4][6]);
        path1_2.append(loc[4][5]);
        path1_2.append(loc[4][4]);
        path1_2.append(loc[5][4]);
        path1_2.append(loc[6][4]);
        path1_2.append(loc[6][3]);
        path1_2.append(loc[6][2]);
        path1_2.append(loc[6][1]);
        path1_2.append(loc[6][0]);


        //path1_3
        path1_3.append(loc[7][11]);
        path1_3.append(loc[7][10]);
        path1_3.append(loc[7][9]);
        path1_3.append(loc[7][8]);
        path1_3.append(loc[7][7]);
        path1_3.append(loc[7][6]);
        path1_3.append(loc[7][5]);
        path1_3.append(loc[7][4]);
        path1_3.append(loc[7][3]);
        path1_3.append(loc[7][2]);
        path1_3.append(loc[7][1]);
        path1_3.append(loc[7][0]);



        //*******************************写死路径***************************************************************************


        //初始化成员变量
        state_num = num;
        path_set.clear();
        switch(state_num)
        {
        case 1:
            {
            path_set.append(path1_1);
            path_set.append(path1_2);
            path_set.append(path1_3);
            //*****将路径添加到路径集********
            }
            break;
        default:
            break;

        }

    }
    //返回某关卡路径集
    QVector<QVector<location>> get_path_set()
    {
        return path_set;
    }


    //测试函数，开发时用来测试这个类的各个功能是否正常
    void test(){
//        QString str = QString("第一关path1_1步数= %1 ").arg(path1_1.size());
//        qDebug()<<str;

    }

};



























#endif // MAP_AND_PATH_H

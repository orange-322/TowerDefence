#include "mainscene.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();
//                             所有文件中带有 [********] 注释标记的代码段是需要注意，可能引起bug的地方（如地图数组大小的统一）
//                                          [########]注释的内容是后期增加东西的时候可以调整的游戏机制（如敌人的生成方案）
//                                          [@@@@@@@@]注释的内容是后期可以增加的花里胡哨的特效音效UI等等
//                                          [!!!!!!!!]是加上远程塔时需要添加的地方
    //[********] 这个项目中所有的像素坐标都是【原始数组坐标】【按map_block_size比例放大】【高度加上start_height_pixel】后的结果 [********]
    //[********]    判断鼠标点击区域时尤其注意这点    [********]


    //gamewindow.cpp中第235行游戏结束时的效果还没做   [@@@@@@@@]

    return a.exec();
}

#ifndef REMOTETOWERSELECTION_H
#define REMOTETOWERSELECTION_H

#include"global_structure.h"

class RemoteTowerSelection//【选择远程防御塔界面】
{
private:

    location position;//整个选择框左上角的像素坐标

    //整个大的选择框的边长，设置成3*3个砖块大小的大矩形
    int side_length = 3*map_block_size;
    //选择框内每种塔的选择按钮大小
    int button_length = map_block_size;

    //选择框的选择状态
    bool show = false;

    //tower_3_selection的图片路径
    QString tower_3_picture_path = "://res/tower_3_selection.png";
    //tower_4_selection的图片路径
    QString tower_4_picture_path = "://res/tower_4_selection.png";
    //tower_5_selection的图片路径
    QString tower_5_picture_path = "://res/tower_5_selection.png";

    //TODO[########]后续这里可以加上其他近战塔的图片路径
    ////////////////////////////////////////////

public:
    RemoteTowerSelection(){position.x = 0;position.y = 0;}//构造函数
    RemoteTowerSelection(location p){position.x = p.x;position.y = p.y;}//构造函数
    bool RemoteTowerSelection_is_showed(){return show;}//是否显示【选择远程防御塔界面】
    void RemoteTowerSelection_show(){show = true;}//置这个选择框为显示状态
    void RemoteTowerSelection_hide(){show = false;}//置这个选择框为隐藏状态


    int get_side_length()const{return side_length;}//返回整个大的选择框的边长
    int get_button_length()const{return button_length;}//返回选择框内每种塔的选择按钮大小
    location get_position()const{return position;}//返回整个选择框的像素坐标
    QString get_tower_3_picture_path()const{return tower_3_picture_path;}//返回3号防御塔的图片路径
    QString get_tower_4_picture_path()const{return tower_4_picture_path;}//返回4号防御塔的图片路径
    QString get_tower_5_picture_path()const{return tower_5_picture_path;}//返回5号防御塔的图片路径

    //TODO[########]后续这里可以加上其他近战塔的图片路径
    ////////////////////////////////////////////

    void set_position(location p){position.x = p.x;position.y = p.y;}//设定这个选择框的坐标(单位是像素，和地图匹配)



};

#endif // REMOTETOWERSELECTION_H

#ifndef MELEETOWERSELECTION_H
#define MELEETOWERSELECTION_H

#include"global_structure.h"

class MeleeTowerSelection//【选择近战防御塔界面】
{
private:

    location position;//整个选择框左上角的像素坐标

    //整个大的选择框的边长，设置成3*3个砖块大小的大矩形
    int side_length = 3*map_block_size;
    //选择框内每种塔的选择按钮大小
    int button_length = map_block_size;

    //选择框的选择状态
    bool show = false;

    //tower_1_selection的图片路径
    QString tower_1_picture_path = "://res/tower_1_selection.png";
    //tower_2_selection的图片路径
    QString tower_2_picture_path = "://res/tower_2_selection.png";
    //TODO[########]后续这里可以加上其他近战塔的图片路径
    ////////////////////////////////////////////

public:
    MeleeTowerSelection(){position.x = 0;position.y = 0;}//构造函数
    MeleeTowerSelection(location p){position.x = p.x;position.y = p.y;}//构造函数
    bool MeleeTowerSelection_is_showed(){return show;}//是否显示【选择近战防御塔界面】
    void MeleeTowerSelection_show(){show = true;}//置这个选择框为显示状态
    void MeleeTowerSelection_hide(){show = false;}//置这个选择框为隐藏状态


    int get_side_length()const{return side_length;}//返回整个大的选择框的边长
    int get_button_length()const{return button_length;}//返回选择框内每种塔的选择按钮大小
    location get_position()const{return position;}//返回整个选择框的像素坐标
    QString get_tower_1_picture_path()const{return tower_1_picture_path;}//返回一号防御塔的图片路径
    QString get_tower_2_picture_path()const{return tower_2_picture_path;}//返回二号防御塔的图片路径
    //TODO[########]后续这里可以加上其他近战塔的图片路径
    ////////////////////////////////////////////

    void set_position(location p){position.x = p.x;position.y = p.y;}//设定这个选择框的坐标(单位是像素，和地图匹配)



};

#endif // MELEETOWERSELECTION_H

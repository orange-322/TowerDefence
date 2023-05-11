#include<QDebug>
#include<QMenuBar>
#include<QTimer>

#include "gamewindow.h"






//GameWindow::GameWindow(QWidget *parent) :
//    QMainWindow(parent)
//{
//}




//宏
double DISTANCE(double x1,double y1,double x2,double y2)//类似于宏，计算(x1,y1)与(x2,y2)的距离
{
    return qSqrt(((x1)-(x2))*((x1)-(x2))+((y1)-(y2))*((y1)-(y2)));
}


//小工具函数，不是gamewindow类成员，判断点击区域(evx,evy)是否在以(x,y)为左上角的宽为width，高为height的矩形框内
bool CLICK_RIGION(int evx,int evy,int x,int y,int width,int height)
{
    return (  evx>x  &&  evx<x+width  &&  evy>y  &&  evy<y+height  );
}


//小工具函数，gamewindow类成员，判断鼠标是否点击了某个物件砖块，有则返回物件代号
//mouseclick_x和mouseclick_y分别对应鼠标点击位置的像素坐标(ev->x和ev->y)
int GameWindow::CLICK_OBJECT(int mouseclick_x,int mouseclick_y)//判断鼠标是否点击了某个物件砖块,将鼠标点击区域和像素地图中的每个地砖对应，再对应到原始数组中判断这是墙、路、要塞还是可以放塔的地方，返回值为    0 墙壁   1 怪物路径   2 怪物出生点   3 防御塔位置   4 堡垒
{
    //遍历原始数组，将每个原始数组下标转化成地图中像素下标【注意行数i对应高pixel_height，列数j对应宽pixel_width】
    for(int i=0;i<MAP_ROW;i++)
        for(int j=0;j<MAP_COL;j++)
        {
            int pixel_width = j*map_block_size;
            int pixel_height = i*map_block_size + start_height_pixel;
            if(mouseclick_x >= pixel_width &&
                    mouseclick_x <= pixel_width + map_block_size &&
                    mouseclick_y >= pixel_height &&
                    mouseclick_y <= pixel_height + map_block_size)
                return map_arr[i][j];
        }
    //没点击到地图上的任何物件就返回-1（可能点击菜单栏或者生命金钱栏，外部调用时就不做任何反应）
    return -1;
}

//小工具函数,gamewindow类成员,返回当前点击的格子的左上角坐标
location GameWindow::CLICK_BLOCK(int mouseclick_x,int mouseclick_y)
{
    location res;
    //遍历原始数组，将每个原始数组下标转化成地图中像素下标【注意行数i对应高pixel_height，列数j对应宽pixel_width】
    for(int i=0;i<MAP_ROW;i++)
        for(int j=0;j<MAP_COL;j++)
        {
            int pixel_width = j*map_block_size;
            int pixel_height = i*map_block_size + start_height_pixel;
            if(mouseclick_x >= pixel_width &&
                    mouseclick_x <= pixel_width + map_block_size &&
                    mouseclick_y >= pixel_height &&
                    mouseclick_y <= pixel_height + map_block_size)
            {
                res.x = pixel_width;
                res.y = pixel_height;
                return res;
            }
        }

    //没点击到地图上的任何有效格子就返回-1（可能点击菜单栏或者生命金钱栏，外部调用时就不做任何反应）
    res.x = -1;res.y=-1;
    return res;
}

    //小工具函数：判断鼠标点击区域是否和传入的塔所在的格子一致，本质就是算【鼠标点击处的像素坐标】和【塔所在的格的像素坐标范围】匹不匹配
bool GameWindow::match_tower(int evx,int evy,TowerBase * tower_item)
{
    return (    evx>tower_item->get_position().x &&
                evx<tower_item->get_position().x + map_block_size &&
                evy>tower_item->get_position().y &&
                evy<tower_item->get_position().y + map_block_size
            );

}

//小工具函数：判断敌人坐标到enemy_4的坐标距离是否在(x*x)/(90*90) + (y*y)/(56*56)== 1这个黄金比例椭圆内
bool IN_OVAL_RANGE(int x0,int y0,int x1,int y1)
{
    double x = x1-x0,y = y1-y0;
    double temp = (x*x)/(90*90) + (y*y)/(56*56);
    if(temp < 1)
        return true;
    else
        return false;
}

//花钱函数
bool GameWindow::cost_money(TowerBase* tower_item)//在防御塔上花钱的函数，假如买不起就return false
{
    if(tower_item == nullptr)
        return false;
    if(Money > tower_item->get_value())//假设买得起
    {
        Money -= tower_item->get_value();//扣钱
        MoneyLabel->setText(QString("%1").arg(Money));
        MoneyLabel->adjustSize();//更新label
        return true;
    }
    else//假如买不起
        return false;
}



//构造函数
GameWindow::GameWindow(int num)
{
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&记得修改这里，global_use.h中更改的内容才会生效！&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
    int make_global_h_recompile = 11;//修改global_use.h后为了重新编译gamewindow.cpp，必须把这个文件也修改一下,故设置工具int
    //&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&记得修改这里，global_use.h中更改的内容才会生效！&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//////////////////////////////////////////////////初始化绘图路径数组//////////////////////////////////////////////////////////
///1.绘制怪物收到攻击时的拟声词特效的图片路径数组
    splash.append(QString("://res/splash1.png"));
    splash.append(QString("://res/splash2.png"));
    splash.append(QString("://res/splash3.png"));
    splash.append(QString("://res/splash4.png"));
    splash.append(QString("://res/splash5.png"));
    splash.append(QString("://res/splash6.png"));


//////////////////////////////////////////////////设置窗口信息///////////////////////////////////////////////////////////////

    //第num关
    QString str = QString("第 %1 关！").arg(num);

//    //调试信息
//    qDebug()<<str;

    //设置窗口信息
    this->setWindowTitle(str);
    this->setFixedSize(720,480+start_height_pixel);//+start_height_pixel是菜单栏遮住了顶部的砖块，故后来把菜单栏的位置空出来
    this->setWindowIcon(QIcon(":/res/window_icon.png"));
    //创建菜单栏
    QMenuBar *bar = menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu * startMenu = bar->addMenu("开始");

    //创建[设置] 菜单项
    QAction * settingAction = startMenu->addAction("设置");


    //创建[退出] 菜单项
    QAction * quitAction = startMenu->addAction("退出");

    //点击[退出] 实现退出游戏
    connect(quitAction,&QAction::triggered,this,[=](){
        this->close();
    });

    //创建返回 菜单项
    QAction * backAction = startMenu->addAction("返回");

    //点击返回 实现回到游戏选择界面
    connect(backAction,&QAction::triggered,this,[=](){
        emit this->GameWindowBack();
    });

    //设置背景音乐
    gamewindow_bgm = new QMediaPlayer(this);
    gamewindow_bgm->setMedia(QUrl::fromLocalFile("D:/Qt/my_own/chap02/TowerDefence/music/gamewindow.mp3"));
    gamewindow_bgm->setVolume(65);

    //实例化游戏页面的设置界面
    gamewindow_settings = new setting_scene;

    //点击菜单栏中的设置实现打开设置界面
    connect(settingAction,&QAction::triggered,this,[=](){
        gamewindow_settings->show();
    });

///////////////////////////////////////////////////设置界面的各项功能连接如下//////////////////////////////////////////////////////////////
    //监听来自设置界面的音量调节signal
    connect(gamewindow_settings,&setting_scene::volume_setting_signal,this,[=](int j){
        gamewindow_bgm->setVolume(j);
    });
///////////////////////////////////////////////////设置界面的各项功能连接如上//////////////////////////////////////////////////////////////





///////////////////////////////////直接从文件读取地图信息。每次在主界面可以选择导出地图文本，这个游戏界面就直接读文本了/////////////////////////////////////////////////////////
    //设置成员变量
    //关卡数
    this->StateNum = num;

    //先把要打开的文件名map_file_path1连好
    QString prefix1 = "map";
    QString surfix1 = ".txt";
    QString temp_state_num1 = QString("%1").arg(this->StateNum);
    QString QS_map_file_path1 = prefix1+temp_state_num1+surfix1;
    char *map_file_path1;
    QByteArray ba1 = QS_map_file_path1.toLatin1();
    map_file_path1 = ba1.data();

    //打开文件
    QFile fpp(map_file_path1);
    fpp.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&fpp);

    //对第一行
    QString line = in.readLine();//读第一行,拆分并转成int
    QString row_s = line.section(',',0,0);
    QString col_s = line.section(',',1,1);//学会了用QFile和QTextStream配合打开文件流，再用section切分，掌控雷电！！！
    int current_row = row_s.toInt(), current_col = col_s.toInt();

    //从文本文件中载入相应关卡的地图到成员map_arr中
    //对地图数组写入
    for(int i=0;i<current_row;i++)
    {
        line = in.readLine();//读取一行
        for(int j=0;j<current_col;j++)
        {
            map_arr[i][j] = line.section(',',j,j).toInt();//把拆分后这一行下标为j的元素转换成int存入数组
        }
    }//此时paintEvent同步绘制地图

    //获取StateNum关的路径集，将其存入path_set中
    //对路径写入
    QVector<QVector<location>> path_set;
    int path_num = in.readLine().toInt();//读出路径总数path_num
    for(int path_idx = 0; path_idx < path_num; path_idx++)//每条路径的index,从0开始
    {
        int step_num = in.readLine().toInt();//读出path_idx这条路径的步数
        QVector<location> single_path;//单条路径
        for(int step_idx = 0; step_idx < step_num; step_idx++)//把单条路径装载
        {   line = in.readLine();//读一行坐标
            location temp;
            temp.x = line.section(',',0,0).toInt();
            temp.y = line.section(',',1,1).toInt();//将其拆分为x,y存入temp
            single_path.append(temp);//把temp塞进这条路径
        }
        path_set.append(single_path);//把single_path塞进总路径集path_set
    }

///////////////////////////////////////////地图读取完毕//////////////////////////////////////////////////////////////////////////

    //由于path_set对应的是原始数组中的坐标，并不是 数组【经map_block_size放大后的】且【加上了start_height_pixel的】地图 中的像素坐标，不利于怪物的移动与绘画
    //故将path_set最内层的坐标全部【按map_block_size放大】、【height加上start_height_pixel】后存入this->path_set_on_map;
    //path_set_on_map为地图上的像素路径
    for(int i=0;i<path_set.size();i++)
    {
        QVector<location> temp1;
        for(int j=0;j<path_set[i].size();j++)
            temp1.append(location(path_set[i][j].y*map_block_size,
                                  start_height_pixel+path_set[i][j].x*map_block_size));//尤其注意原地图坐标[行][列]和像素坐标[宽][高]的表示是相反的,【行】对应【高】,【列】对应【宽】,故原本【x代表的行数】就应该换算成【高】
        path_set_on_map.append(temp1);
    }


//////////////////////////////////////////////各类标签的显示///////////////////////////////////////////////////////////////////////
    //显示生命图标
    PlayerHealthIcon->move(5,26);
    QImage *health_icon = new QImage;
    health_icon->load("://res/health_label.png");
    PlayerHealthIcon->setPixmap(QPixmap::fromImage(*health_icon));

    //显示生命标签
    PlayerHealthLabel->move(50,29);
    PlayerHealthLabel->setFont(QFont("黑体",15));
    PlayerHealthLabel->setText(QString("%1").arg(PlayerHealth));
    PlayerHealthLabel->adjustSize();


    //显示金钱图标
    MoneyIcon->move(290,25);
    QImage *money_icon = new QImage;
    money_icon->load("://res/money_label.png");
    MoneyIcon->setPixmap(QPixmap::fromImage(*money_icon));

    //显示金钱标签
    MoneyLabel->move(345,29);
    MoneyLabel->setFont(QFont("黑体",15));
    MoneyLabel->setText(QString("%1").arg(Money));
    MoneyLabel->adjustSize();

//timer_1
    //计时器timer_1用来实时更新生命与金钱的Label
        QTimer* timer_1 = new QTimer(this);
        timer_1->start(100);//每0.1秒
        connect(timer_1,&QTimer::timeout,this,[=](){
            PlayerHealthLabel->setText(QString("%1").arg(PlayerHealth));
            PlayerHealthLabel->adjustSize();
            Money+=100;//资源随时间流逝而增加
            MoneyLabel->setText(QString("%1").arg(Money));
            MoneyLabel->adjustSize();
        });

    //显示游戏时长图标
    TimeIcon->move(555,26);
    QImage *time_icon = new QImage;
    time_icon->load("://res/clock_label.png");
    TimeIcon->setPixmap(QPixmap::fromImage(*time_icon));

    //在游戏界面显示出游戏时长
    TimeLabel->move(595,29);
    TimeLabel->setFont(QFont("黑体",15));
    TimeLabel->setText(QString("%1s").arg(GameTime));
    TimeLabel->adjustSize();

//timer0
//计时器time0配合成员变量second告诉现在是游戏开始第second秒（迪亚波罗狂喜）
    QTimer* timer0 = new QTimer(this);
    timer0->start(1000);
    connect(timer0,&QTimer::timeout,this,[=](){
        GameTime++;
        //更新显示游戏时长的标签:每过一秒setText一次就行
        TimeLabel->setText(QString("%1s").arg(GameTime));
        TimeLabel->adjustSize();
    });


//////////////////////////////////////接下来在起始位置生成敌人，并加入BirthVector中/////////////////////////////////////////////////
//timer1
//设定定时器timer1,timer1控制敌人的生成这一生态循环
    QTimer* timer1 = new QTimer(this);
    timer1->start(GENERATE_ENEMY_INTERVAL);    //每隔GENERATE_ENEMY_INTERVAL秒就在出生点加入敌人（加入敌人数组）
    BirthVector.clear();//把目前存在的敌人清空，避免切换关卡时怪物仍然存在
    OnStageVector.clear();

    connect(timer1,&QTimer::timeout,this,[=]()mutable//为了能修改观察到的变量，设置这个lambda表达式为mutable
    {
        /////////////////////////////////////////////////////////根据该关卡的路径数定制不同的怪物产生方案
        switch (path_set_on_map.size()/*当前地图的路径数量*/) {
        case 1:
            {
                if(GameTime>0 && GameTime < 5)
                {
                    BirthVector.push_back(new Enemy_1(path_set_on_map[0]));//在第一条路径起点生成一个1号敌人
                }
//后期加入更多种类的敌人，这里就可以完善5秒后更多种类的敌人的生成了，case2 case3同理                                       [########]
            }
            break;
        case 2:
            {
                if(GameTime>0 && GameTime < 3)
                {
                    BirthVector.push_back(new Enemy_1(path_set_on_map[0]));//在第一条路径起点生成一个1号敌人
                }
                else if(GameTime <6)
                {
                    BirthVector.push_back(new Enemy_2(path_set_on_map[0]));//在第一条路径起点生成一个2号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[1]));//在第二条路径起点生成一个1号敌人
                }
                else if(GameTime<10)
                {
                    BirthVector.push_back(new Enemy_1(path_set_on_map[0]));//在第一条路径起点生成一个1号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[1]));//在第二条路径起点生成一个2号敌人
                }
                else if(GameTime<14)
                {
                    BirthVector.push_back(new Enemy_2(path_set_on_map[0]));//在第一条路径起点生成一个2号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[1]));//在第二条路径起点生成一个1号敌人
                }
                else if(GameTime>17 &&GameTime <19)
                {
                    BirthVector.push_back(new Enemy_3(path_set_on_map[0]));//在第一条路径起点生成一个3号敌人
                    BirthVector.push_back(new Enemy_3(path_set_on_map[1]));//在第二条路径起点生成一个3号敌人
                }

            }
            break;
        case 3:
            {
                if(GameTime>0 && GameTime < 3)
                {
                    BirthVector.push_back(new Enemy_1(path_set_on_map[0]));//在第一条路径起点生成一个1号敌人
                }
                else if(GameTime <6)
                {
                    BirthVector.push_back(new Enemy_2(path_set_on_map[0]));//在第一条路径起点生成一个2号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[1]));//在第二条路径起点生成一个1号敌人
                }
                else if(GameTime <7)
                {
                    BirthVector.push_back(new Enemy_4(path_set_on_map[0]));//在第一条路径起点生成一个4号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[1]));//在第二条路径起点生成一个1号敌人
                }
                else if(GameTime<10)
                {
                    BirthVector.push_back(new Enemy_1(path_set_on_map[0]));//在第一条路径起点生成一个1号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[1]));//在第二条路径起点生成一个1号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[2]));//在第三条路径起点生成一个1号敌人
                }
                else if(GameTime<11)
                {
                    BirthVector.push_back(new Enemy_4(path_set_on_map[0]));//在第一条路径起点生成一个4号敌人
                    BirthVector.push_back(new Enemy_4(path_set_on_map[1]));//在第二条路径起点生成一个4号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[2]));//在第三条路径起点生成一个1号敌人
                }
                else if(GameTime<14)
                {
                    BirthVector.push_back(new Enemy_2(path_set_on_map[0]));//在第一条路径起点生成一个2号敌人
                    BirthVector.push_back(new Enemy_1(path_set_on_map[1]));//在第二条路径起点生成一个1号敌人
                    BirthVector.push_back(new Enemy_2(path_set_on_map[2]));//在第三条路径起点生成一个2号敌人
                }
                else if(GameTime>17 &&GameTime <19)
                {
                    BirthVector.push_back(new Enemy_3(path_set_on_map[0]));//在第一条路径起点生成一个3号敌人
                    BirthVector.push_back(new Enemy_3(path_set_on_map[1]));//在第二条路径起点生成一个3号敌人
                    BirthVector.push_back(new Enemy_3(path_set_on_map[2]));//在第三条路径起点生成一个3号敌人
                }


//后期加入更多路径条数的地图，这里就可以加入case4 case5等更多case了                                                  [########]
            }
            break;
        default:
            break;
        }

//        //test
//        QString u1 = QString("现在场上存在%1个近战塔").arg(MeleeTowerVector.size());
//        qDebug()<<u1;
    });

////////////////////////////////////////////游戏开始进行///////////////////////////////////////////////////////////////////////////
//timer2控制游戏的functioning
    QTimer *timer2 = new QTimer(this);
    timer2->start(100);//0.1秒推动游戏变化一次,相当于游戏帧率

    connect(timer2,&QTimer::timeout,this,[&](){


        //1.敌人运作
        //1.(1)敌人的移动
        //将birth阶段的敌人移动搬运到on_stage阶段中
        if(!BirthVector.isEmpty()){//注意在前面要判断一下
            for(auto enemy_item = BirthVector.begin();enemy_item != BirthVector.end();){
                OnStageVector.append((*enemy_item));
                auto temp = enemy_item;
                BirthVector.erase(temp);//erase之后迭代器会自动指向下一个元素的位置，故for循环内不应再次让迭代器自增
            }
        }
        //遍历on_stage的每个敌人，对其进行移动操作
        for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();)
        {
            //判断敌人是否 【移动到终点】 或 【血量减到<=0】，若满足其中之一就将其从on_stage阶段移除(伟大的erase函数让我能够把vector当链表用，Hooray!)
            if(!(*enemy_item)->move()   || (*enemy_item)->get_current_hp()<=0)
            {
                bool game_is_over = false;
                if(!(*enemy_item)->move())//【敌人走到了终点】
                {
                    //减少玩家生命值
                    PlayerHealth--;//可以根据不同怪设置减少不同的玩家生命值   [########]


                    //如果玩家生命值减少至<=0则游戏失败
                    if(PlayerHealth<=0)
                    {
                        //游戏结束的相关处理
                        game_is_over = true;
                        EnemyBase* del = (*enemy_item);
                        OnStageVector.erase(enemy_item);//将这个敌人从OnStageVector中删去
                        delete del;//释放空间
                        //test
                        //目前只是debug一下并退出窗口，以后可以调成别的特效+按钮
                        qDebug()<<"游戏结束！";
                        //this->close();
                        ////////////这里后期可以加上对游戏的暂停、游戏失败图标的显示、返回关卡选择界面的按钮显示等等    [@@@@@@@@]
                        //test
                    }
                }
                if((*enemy_item)->get_current_hp()<=0)//【敌人血量减到了0】
                {
                    //后期可以加入敌人死亡的特效paint                                                [@@@@@@@@]

                }
                if(!game_is_over)
                {
                    EnemyBase* del = (*enemy_item);
                    OnStageVector.erase(enemy_item);//最后如果游戏还没结束再将这个敌人从OnStageVector中删去
                    delete del;//释放空间
                }
            }
            else{
                enemy_item++;//与erase形成的迭代器自增相配合，for循环语句()内不写自增
            }
        }
        //////////////////////////////利用前置声明让敌人的attack和塔的attack独立起来
        //1.(2)敌人的攻击
        //遍历on_stage的每个敌人
        for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();enemy_item++)
        {
                QVector<TowerBase*> tower_in_range;
                //遍历MeleeTowerVector的每个塔，将敌人攻击范围内的近战塔收集起来 送给该敌人 敌人对这部分防御塔发出攻击
                for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();melee_item++)
                {
                    if(DISTANCE((*melee_item)->get_position().x,
                                (*melee_item)->get_position().y,
                                (*enemy_item)->get_position().x,
                                (*enemy_item)->get_position().y
                                )
                            <=  (*enemy_item)->get_attack_range())//如果二者的左上角像素坐标距离 < 防御塔的攻击范围
                        tower_in_range.append(*melee_item);//BUG这里不知道这种值传递能不能使OnStage里的对象真正掉血[********]
                }
                (*enemy_item)->Attack(tower_in_range,GameTime);//敌人对塔的攻击(tower_in_range为空也要传入，因为要在attack函数中调整恋战敌人的速度)
        }
        //1.(3)场上所有enemy_4法老对黄金比例椭圆范围内的友方回血
        for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();enemy_item++)
        {
            (*enemy_item)->is_healing = false;//把正在回血状态全部清0，由下面的Guard函数对特定的对象设置回血状态1，最后再repaint
        }
        //遍历on_stage的所有第4类敌人法老
        for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();enemy_item++)
        {
            if((*enemy_item)->get_type() == 4)//如果是第4类敌人法老
            {
                QVector<EnemyBase*> enemy_in_range;
                //遍历on_stage的友方敌人，将黄金椭圆攻击范围内的友方敌人收集起来 送给该法老 法老对这部分敌人治疗
                for(auto friend_enemy = OnStageVector.begin();friend_enemy!=OnStageVector.end();friend_enemy++)
                {
                    if(IN_OVAL_RANGE((*enemy_item)->get_position().x,
                                (*enemy_item)->get_position().y,
                                (*friend_enemy)->get_position().x,
                                (*friend_enemy)->get_position().y
                                ))//如果在椭圆内
                        enemy_in_range.append(*friend_enemy);//BUG这里不知道这种值传递能不能使OnStage里的对象真正掉血[********]
                }
                (*enemy_item)->Guard(enemy_in_range,GameTime);//法老对友方敌人的治疗(enemy_in_range为空也要传入)

            }
        }

        //2.防御塔运作
        //2.(1)近战塔攻击
        for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();enemy_item++)
        {(*enemy_item)->is_being_attacked = -1;}//先置每个敌人为未受攻击状态(置为-1)，经历下面每个塔的Attack函数后确定当前的受攻击状态(置为0~5)
        for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();melee_item++)
        {//遍历每个近战塔
            QVector<EnemyBase*> enemy_in_range;
            //遍历on_stage的每个敌人，将防御塔攻击范围内的敌人收集起来 送给该防御塔 防御塔对这部分敌人发出攻击
            for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();enemy_item++)
            //for(auto &enemy_item : OnStageVector)
            {
                if(DISTANCE((*enemy_item)->get_position().x,
                            (*enemy_item)->get_position().y,
                            (*melee_item)->get_position().x,
                            (*melee_item)->get_position().y
                            )
                        <=  (*melee_item)->get_attack_range())//如果二者的左上角像素坐标距离 < 防御塔的攻击范围
                    enemy_in_range.append(*enemy_item);//BUG这里不知道这种值传递能不能使OnStage里的对象真正掉血[********]
            }
//            //test
//            QString yy = QString("当前防御塔攻击范围内的敌人有 %1 个").arg(enemy_in_range.size());
//            if(!enemy_in_range.isEmpty())
//                qDebug()<<yy;
//            if(!enemy_in_range.isEmpty())//塔的攻击范围内有敌人才攻击
              (*melee_item)->Attack(enemy_in_range,GameTime);//塔的攻击(范围内enemy_in_range为空也要传入，因为要调整攻击特效)
        }
        //检查近战塔的消亡(逻辑上在Attack函数调用之后，这样就可以在Attack函数中操作)
        for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();)
        {//遍历每个近战塔
            if((*melee_item)->get_current_hp()<=0)
            {
                TowerBase* del = (*melee_item);
                MeleeTowerVector.erase(melee_item);//如果有血量<=0的近战塔就将其从近战塔数组中删除
                delete del;//释放空间
            }
            else
                melee_item++;//注意这里erase的话迭代器就不自增，仅在没有调用erase的情况下迭代器才自增
        }

        //2.(2)远程塔攻击                                                                                  [########][********](还没写)[!!!!!!!!]
        //TODO





        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //3.判断游戏胜利
        if(GameTime>0 && BirthVector.isEmpty() && OnStageVector.isEmpty() && PlayerHealth > 0)//出生数组、on_stage数组均空，且玩家生命>0就胜利了
        {//注意这段代码在每一帧中会循环执行，如果要让游戏停下来最好设个标志位啥的并且让下面的update()不执行[********]
            //test
            //目前只是debug一下并退出窗口，以后可以调成别的特效+按钮
            qDebug()<<"游戏胜利！";
            //this->close();
            ////////////这里后期可以加上对游戏的暂停、游戏胜利图标的显示、返回关卡选择界面的按钮显示等等    [@@@@@@@@]
            //TODO
            //test
        }

        //4.重新绘图(标准动作)
        update();
    });

    ///////////////////子弹的移动
    //timer2_1控制游戏的functioning中子弹移动的部分
    QTimer *timer2_1 = new QTimer(this);
    timer2_1->start(10);//0.01秒推动子弹变化一次
    connect(timer2_1,&QTimer::timeout,this,[&](){
        //1.子弹的移动
        for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();enemy_item++)
        {
            (*enemy_item)->tower_bullet_move();
        }
        //2.检查消除敌人
        for(auto enemy_item = OnStageVector.begin();enemy_item!=OnStageVector.end();)
        {
            if((*enemy_item)->get_current_hp() <= 0)
            {
                EnemyBase* del = (*enemy_item);
                OnStageVector.erase(enemy_item);
                delete del;
            }
            else
                enemy_item++;
        }
        //3.重新绘图(标准动作)
        update();
    });

}

//析构函数
GameWindow::~GameWindow()
{
    //////////////开发期间看有哪些资源在游戏界面退出时要释放
    delete TimeLabel;
    delete TimeIcon;
    delete MoneyLabel;
    delete MoneyIcon;
    delete PlayerHealthLabel;
    delete PlayerHealthIcon;
    delete gamewindow_bgm;
    delete gamewindow_settings;
    //清除出生阶段的敌人
    for(auto it = BirthVector.begin();it!=BirthVector.end();)
    {
        EnemyBase* del = (*it);
        BirthVector.erase(it);
        delete del;
    }
    //清除当前地图上存在的敌人
    for(auto it = OnStageVector.begin();it!=OnStageVector.end();)
    {
        EnemyBase* del = (*it);
        BirthVector.erase(it);
        delete del;
    }
    //清除目前场上存在的防御塔
    for(auto it = MeleeTowerVector.begin();it!=MeleeTowerVector.end();)
    {
        TowerBase* del = (*it);
        MeleeTowerVector.erase(it);
        delete del;
    }

}



///////////////////////////////////////////////////////正在写这里///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// 需要能够修改MAP_ROW和MAP_COL，同时对于map_arr也不能全局，得在这里生成动态的
//绘制地图
void GameWindow::paintMap(QPainter& painter)
{
    //地砖数组中的每个元素相当于一块地砖
    //对所有的地砖分别进行QPixmap绘图
    for(int i=0;i<MAP_ROW;i++){
        for(int j=0;j<MAP_COL;j++)
        {
            //根据这块地砖的类型用不同的资源图片绘画
            switch(map_arr[i][j])
            {
            case WALL://这格是墙壁
//                {
//                QPixmap temp;
//                temp.load(QString(WALL_PIC_PATH));
//                painter.drawPixmap(j*map_block_size,i*map_block_size,map_block_size,map_block_size,temp);
//                }
                //上下两段drawPixmap代码是等价的，但是出现了很诡异的bug:在调整全局map_block_size的时候必须要切换一下写法才能更新砖块大小，否则砖块大小跟上次编译一样，很迷    [******]
                {painter.drawPixmap(j*map_block_size,start_height_pixel+i*map_block_size,map_block_size,map_block_size,QPixmap(WALL_PIC_PATH));}
                break;
            case PATH:case BIRTH://这格是路径或怪物出生点
//                {
//                QPixmap temp;
//                temp.load(QString(PATH_PIC_PATH));
//                painter.drawPixmap(j*map_block_size,i*map_block_size,map_block_size,map_block_size,temp);
//                }
                {painter.drawPixmap(j*map_block_size,start_height_pixel+i*map_block_size,map_block_size,map_block_size,QPixmap(PATH_PIC_PATH));}
                break;
            case TOWER://这格是防御塔可放置的位置
//                {
//                QPixmap temp;
//                temp.load(QString(TOWER_PIC_PATH));
//                painter.drawPixmap(j*map_block_size,i*map_block_size,map_block_size,map_block_size,temp);
//                }
                {painter.drawPixmap(j*map_block_size,start_height_pixel+i*map_block_size,map_block_size,map_block_size,QPixmap(TOWER_PIC_PATH));}
                break;
            case FORTRESS://这格是堡垒要塞
//                {
//                QPixmap temp;
//                temp.load(QString(FORTRESS_PIC_PATH));
//                painter.drawPixmap(j*map_block_size,i*map_block_size,map_block_size,map_block_size,temp);
//                }
                {painter.drawPixmap(j*map_block_size,start_height_pixel+i*map_block_size,map_block_size,map_block_size,QPixmap(FORTRESS_PIC_PATH));}
                break;

            }


        }
    }
}

//绘制enemy_4法老守卫特效
void GameWindow::paintEnemy4GuardingEffect(QPainter& painter)
{
    for(auto enemy_item = OnStageVector.begin();enemy_item != OnStageVector.end();enemy_item++)//遍历场上所有敌人
    {
        //1.画回血"+++"的特效
        if((*enemy_item)->is_healing)
        {
            //如果这个敌人正在回血,就画上加血的特效
            painter.drawPixmap((*enemy_item)->get_position().x,
                               (*enemy_item)->get_position().y,
                               map_block_size,
                               map_block_size,
                               QPixmap(QString("://res/heal_effect.png"))
                               );
        }
        //2.画法老的guard_circle
        if((*enemy_item)->get_type()==4
                && (*enemy_item)->is_guarding)
        {
            //qDebug()<<"法老正在守卫";
            painter.drawPixmap((*enemy_item)->get_position().x-map_block_size,
                               (*enemy_item)->get_position().y-map_block_size,
                               3*map_block_size,
                               3*map_block_size,
                               QPixmap(QString("://res/guard_circle.png"))
                               );
        }

    }
}

//绘制OnStageVector中的敌人
void GameWindow::paintEnemy(QPainter& painter)
{
    for(auto enemy_item = OnStageVector.begin();enemy_item != OnStageVector.end();enemy_item++){
        painter.drawPixmap((*enemy_item)->get_position().x,(*enemy_item)->get_position().y,(*enemy_item)->get_width(),(*enemy_item)->get_height(),QPixmap((*enemy_item)->get_picture_path()));
    }
}

//绘制【选择近战防御塔界面】
void GameWindow::paintMeleeTowerSelection(QPainter& painter)
{
    if(melee_tower_selection.MeleeTowerSelection_is_showed())
    {
        painter.drawPixmap(melee_tower_selection.get_position().x + map_block_size,//从选择框的左上角顶点开始，往右移一个block开始绘制TOWER_1选择按钮
                           melee_tower_selection.get_position().y ,
                           map_block_size,
                           map_block_size,
                           QPixmap(melee_tower_selection.get_tower_1_picture_path())
                           );
        painter.drawPixmap(melee_tower_selection.get_position().x + 2*map_block_size,//从选择框的左上角顶点开始，往右移一个block开始绘制TOWER_2选择按钮
                           melee_tower_selection.get_position().y ,
                           map_block_size,
                           map_block_size,
                           QPixmap(melee_tower_selection.get_tower_2_picture_path())
                           );
        //[########]后期加入新的近战攻击按钮时在这里添加绘制
    }
}
void GameWindow::paintMeleeTowerUpdate(QPainter& painter)//绘制近战防御塔【升级拆除界面】
{
    for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();melee_item++)
    {
        //如果这个塔的update_remove界面正在显示
        if((*melee_item)->update_remove_is_showed())
        {
            //画出存在的remove按钮
            painter.drawPixmap((*melee_item)->get_position().x+map_block_size,
                               (*melee_item)->get_position().y,
                               map_block_size,
                               map_block_size,
                               QPixmap("://res/remove.png")
                               );//在这个塔的左边画出删除按钮
            //画出塔的攻击范围
            painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::Qt4CompatiblePainting);//设置抗锯齿
            QPoint left_up((*melee_item)->get_position().x+map_block_size/2 - (*melee_item)->get_attack_range()
                           ,(*melee_item)->get_position().y+map_block_size/2 - (*melee_item)->get_attack_range());//攻击范围圆的外切正方形的左上角
            QPoint right_down((*melee_item)->get_position().x+map_block_size/2 + (*melee_item)->get_attack_range()
                           ,(*melee_item)->get_position().y+map_block_size/2 + (*melee_item)->get_attack_range());//攻击范围圆的外切正方形的右下角
            QRect drawrect = QRect(left_up,right_down);//攻击范围圆的外切正方形

            QRegion region(drawrect,QRegion::Ellipse);//用这个外切正方形框出一个大圆
            drawrect.setSize(QSize(drawrect.width() - 4,drawrect.height() - 4));//将这个外切正方形长宽缩短4pixel，用来画内部的圆
            drawrect.moveTopLeft(QPoint((*melee_item)->get_position().x+map_block_size/2 - (*melee_item)->get_attack_range() + 2
                                        ,(*melee_item)->get_position().y+map_block_size/2 - (*melee_item)->get_attack_range() + 2));//将这个缩小的正方形向右向下各移动2pixel，以至于和原来的大正方形同心
            QRegion region2(drawrect,QRegion::Ellipse);//用缩小后的正方形框出一个小圆
            painter.setClipRegion(region.subtracted(region2));//核心：使用裁剪，从大圆中抠掉小圆，中间留下的环形剪裁区才能被颜色fill up
            painter.fillRect(region.boundingRect(),Qt::yellow);//在环形剪裁的Region中fill黄色，整体上看这个环形就像一个圆的边


//            ///下面是起初的失败尝试（圆弧是条线，无法着色，圆又只能画成一个饼，我需要画的其实是一个圆环，在圆环内外径中间图上金色显得像一个圆形。
//            QBrush gold_brush( QColor("#FFFF00") );//金刷子
//            painter.setBrush(gold_brush);//应用金刷子
//            QPoint circle_center((*melee_item)->get_position().x+map_block_size/2,(*melee_item)->get_position().y+map_block_size/2);
//            painter.drawEllipse(circle_center,
//                                (int)((*melee_item)->get_attack_range()),
//                                (int)((*melee_item)->get_attack_range())
//                                );


        }

    }
}


//绘制子弹
void GameWindow::paintTowerBullet(QPainter& painter)
{
    for(auto enemy_item = OnStageVector.begin();enemy_item != OnStageVector.end();enemy_item++)
    {
        for(auto tower_bullet_item = (*enemy_item)->tower_bullet_vec.begin();
                        tower_bullet_item != (*enemy_item)->tower_bullet_vec.end();
                                                            tower_bullet_item++)
        {
            painter.drawPixmap((*tower_bullet_item)->x(),
                               (*tower_bullet_item)->y(),
                               20,
                               20,
                               QPixmap(QString("://res/bullet_1.png"))
                               );
        }
    }
}

//绘制血条
void GameWindow::paintHealthBar(QPainter& painter)
{
    QBrush red_brush( QColor("#660000") );//红刷子
    QBrush green_brush( QColor("#00CC33") );//绿刷子

    //////////////////////////////////////绘制防御塔的血条
    for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();melee_item++)
    {
        painter.setBrush(red_brush);//应用红刷子绘制底色
        painter.drawRect((*melee_item)->get_position().x + 10,
                            (*melee_item)->get_position().y - 5,
                            40,
                            5);//绘制矩形
        painter.setBrush(green_brush);//应用绿刷子绘制血量
        double rate = (*melee_item)->get_current_hp()
                        /(*melee_item)->get_full_hp();//计算比例

        painter.drawRect((*melee_item)->get_position().x + 10,
                            (*melee_item)->get_position().y - 5,
                            rate*40,
                            5);//绘制矩形
    }

    //////////////////////////////////////绘制敌人的血条
    for(auto enemy_item = OnStageVector.begin();enemy_item != OnStageVector.end();enemy_item++)
    {
        painter.setBrush(red_brush);//应用红刷子绘制底色
        painter.drawRect((*enemy_item)->get_position().x + 10,
                            (*enemy_item)->get_position().y - 5,
                            40,
                            5);//绘制矩形
        painter.setBrush(green_brush);//应用绿刷子绘制血量
        double rate = (*enemy_item)->get_current_hp()
                        /(*enemy_item)->get_full_hp();//计算比例

        painter.drawRect((*enemy_item)->get_position().x + 10,
                            (*enemy_item)->get_position().y - 5,
                            rate*40,
                            5);//绘制矩形
    }


}


//绘制防御塔攻击特效函数1：防御塔图片上层的特效
void GameWindow::paintTowerAttackEffect_above(QPainter& painter)
{
    for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();melee_item++)
    {
        //tower4的攻击特效
        if((*melee_item)->get_type()==4
                && (*melee_item)->is_attacking)
        {
            //qDebug()<<"防御塔4正在攻击";
            painter.drawPixmap((*melee_item)->get_position().x,
                               (*melee_item)->get_position().y,
                               map_block_size,
                               map_block_size,
                               QPixmap(QString("://res/tower_4_effect2.png"))
                               );
        }
        //tower5的攻击特效
        if((*melee_item)->get_type()==5
                && (*melee_item)->is_attacking)
        {
            //qDebug()<<"防御塔5正在攻击";
            painter.drawPixmap((*melee_item)->get_position().x,
                               (*melee_item)->get_position().y,
                               map_block_size,
                               map_block_size,
                               QPixmap(QString("://res/tower_5_effect.png"))
                               );


        }
        ////////////////////////////////////更多防御塔的特效往这加
    }
}
//绘制防御塔攻击特效函数2：防御塔图片下层的特效
void GameWindow::paintTowerAttackEffect_beneath(QPainter& painter)
{
    for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();melee_item++)
    {
        //tower3的攻击特效
        if((*melee_item)->get_type()==3
                && (*melee_item)->is_attacking)
        {
            //qDebug()<<"防御塔3正在攻击";
            painter.drawPixmap((*melee_item)->get_position().x-map_block_size,
                               (*melee_item)->get_position().y-map_block_size,
                               3*map_block_size,
                               3*map_block_size,
                               QPixmap(QString("://res/tower_3_effect.png"))
                               );
        }
        //tower4的攻击特效
        if((*melee_item)->get_type()==4
                && (*melee_item)->is_attacking)
        {
            //qDebug()<<"防御塔4正在攻击";
            painter.drawPixmap((*melee_item)->get_position().x-map_block_size,
                               (*melee_item)->get_position().y-map_block_size,
                               3*map_block_size,
                               3*map_block_size,
                               QPixmap(QString("://res/tower_4_effect.png"))
                               );
        }

        ////////////////////////////////////更多防御塔的特效往这加

    }
}


//绘制敌人收到攻击时的拟声词特效
void GameWindow::paintEnemyBeingAttackedEffect(QPainter& painter)
{
    for(auto enemy_item = OnStageVector.begin();enemy_item != OnStageVector.end();enemy_item++)
    {
        if((*enemy_item)->is_being_attacked != -1)
        {
            painter.drawPixmap((*enemy_item)->get_position().x + 20,
                               (*enemy_item)->get_position().y - 10,
                               60,
                               15,
                               QPixmap(splash[(*enemy_item)->is_being_attacked])
                               );
        }
    }
}

//绘制近战防御塔
void GameWindow::paintMeleeTower(QPainter& painter)
{
    for(auto melee_item = MeleeTowerVector.begin();melee_item!=MeleeTowerVector.end();melee_item++)
    {   //画出近战塔
        painter.drawPixmap((*melee_item)->get_position().x,
                           (*melee_item)->get_position().y,
                           (*melee_item)->get_width(),
                           (*melee_item)->get_height(),
                           QPixmap((*melee_item)->get_picture_path())
                           );

    }

}



//绘制【选择远程防御塔界面】                                                 [########][********](还没写)[!!!!!!!!]
//TODO
void GameWindow::paintRemoteTowerSelection(QPainter& painter)
{
    if(remote_tower_selection.RemoteTowerSelection_is_showed())
    {
        painter.drawPixmap(remote_tower_selection.get_position().x + map_block_size,//从选择框的左上角顶点开始，往右移一个block开始绘制TOWER_3选择按钮
                           remote_tower_selection.get_position().y ,
                           map_block_size,
                           map_block_size,
                           QPixmap(remote_tower_selection.get_tower_3_picture_path())
                           );
        painter.drawPixmap(remote_tower_selection.get_position().x + 2*map_block_size,//从选择框的左上角顶点开始，往右移2个block、往下移1个block，开始绘制TOWER_4选择按钮
                           remote_tower_selection.get_position().y + map_block_size,
                           map_block_size,
                           map_block_size,
                           QPixmap(remote_tower_selection.get_tower_4_picture_path())
                           );
        painter.drawPixmap(remote_tower_selection.get_position().x,//从选择框的左上角顶点开始，往下移1个block，开始绘制TOWER_5选择按钮
                           remote_tower_selection.get_position().y + map_block_size,
                           map_block_size,
                           map_block_size,
                           QPixmap(remote_tower_selection.get_tower_5_picture_path())
                           );

        //[########]后期加入新的远程攻击按钮时在这里添加绘制
    }
}


//绘制远程防御塔                                                           [########][********](还没写)[!!!!!!!!]
//TODO




//绘图总函数
void GameWindow::paintEvent(QPaintEvent *)
{//注意绘画的先后顺序，后面执行的函数在画最上层
    //实例化画板对象 this指定绘图设备
    QPainter painter(this);
    //把地图画出来
    paintMap(painter);

    //把塔底下的攻击特效画出来
    paintTowerAttackEffect_beneath(painter);
    //把防御塔画出来
    paintMeleeTower(painter);
    //把塔上方的攻击特效画出来
    paintTowerAttackEffect_above(painter);
    //把敌人画出来
    paintEnemy(painter);
    //把法老回血特效画出来
    paintEnemy4GuardingEffect(painter);

    //把子弹画出来
    paintTowerBullet(painter);
    //把血条画出来
    paintHealthBar(painter);
    //把敌人受到攻击时的拟声词特效画出来
    paintEnemyBeingAttackedEffect(painter);

    //把塔的选择界面画出来(如果有的话)
    paintMeleeTowerSelection(painter);
    paintRemoteTowerSelection(painter);

    //把塔的升级移除界面画出来(如果有的话)
    paintMeleeTowerUpdate(painter);


}

//鼠标点击事件重写
void GameWindow::mousePressEvent(QMouseEvent* ev)//每次点击鼠标时
{
    //判断点击了鼠标的哪个键
    if(ev->button() != Qt::LeftButton)//点击的不是鼠标左键
    {
        //////////////////后续加入鼠标右键的点击的相关操作       [########]
    }
    else//点击的是鼠标左键
    {
/*总体而言的思路：
 鼠标点击时要遵循几条rules:
    每种界面（无论是【选择防御塔界面】还是【升级拆除界面】，在任意一次点击中只能显示其中一个， 点击其他地方时，另一个地方之前点击显示的界面要hide（用设置flag的方式吧）
    【注意：【升级拆除界面】因涉及一个塔的数值更改，所以必须和塔的对象绑定。但【选择近战防御塔】和【选择远程防御塔】不用，选择防御塔只用传递像素就行】

    用伪switch代码表示的话就是:
        点击有含义的物体砖块时:
            点击了WALL:    点击了BIRTH:   点击了FORTRESS:
                清除之前的显示状态（【升级拆除界面】隐藏，【选择近战防御塔界面】隐藏，【选择远程防御塔界面】也隐藏）
            点击了PATH：
                如果这块PATH砖上没有近战塔，就把【选择近战防御塔界面】显示，把【选择远程防御塔界面】隐藏，把场上所有塔的【升级拆除界面】隐藏;
                如果这块PATH砖上有近战塔，就在该位置上把【升级拆除界面】显示，把【选择近战防御塔界面】隐藏，把【选择远程防御塔界面】隐藏;
            点击了TOWER：
                如果这块TOWER砖上没有远程塔，就把【选择远程防御塔界面】显示，把【选择近战防御塔界面】隐藏，把场上所有塔的【升级拆除界面】隐藏;
                如果这块TOWER砖上有近战塔，就在该位置上把【升级拆除界面】显示，把【选择近战防御塔界面】隐藏，把【选择防御塔界面】隐藏;
    涉及的几个基本变量:
        QVector<TowerBase*> MeleeTowerVector //装目前场上存在的近战塔,是gamewindow类的成员
        QVector<TowerBase*> RemoteTowerVector//目前场上存在的远程塔,是gamewindow类的成员                      [########][********](还没写)[!!!!!!!!]

        bool MeleeTowerSelection_is_showed()//是否显示【选择近战防御塔界面】,是meleetowerselection类的成员
        void MeleeTowerSelection_show(){show = true;}//置这个选择框为显示状态,是meleetowerselection类的成员
        void MeleeTowerSelection_hide(){show = false;}//置这个选择框为隐藏状态,是meleetowerselection类的成员

        bool RemoteTowerSelection_is_showed()//是否显示【选择远程防御塔界面】,是remotetowerselection类的成员           [########][********](还没写)[!!!!!!!!]
        void do_show_RemoteTowerSelection()//把【选择远程防御塔界面】显示,是remotetowerselection类的成员               [########][********](还没写)[!!!!!!!!]
        void hide_RemoteTowerSelection()//把【选择远程防御塔界面】隐藏,是remotetowerselection类的成员                  [########][********](还没写)[!!!!!!!!]

        bool update_remove_is_showed()//是否显示【升级拆除界面】,是towerbase类的成员
        void do_show_update_remote(){update_remove_show = true;}//将这个塔的【升级拆除界面】设置为显示,是towerbase类的成员
        void hide_update_remote(){update_remove_show = false;}//将这个塔的【升级拆除界面】设置为隐藏,是towerbase类的成员

    【注意：标准动作:每种情况处理完就直接update()加上return，结束此次鼠标事件的处理】

    后来察觉到一个逻辑问题，由于【选择远程防御塔界面】【升级拆除界面】会覆盖在空砖块区域上，点击的区域和空砖块是重叠的，故代码需要重构。
    必须先判断【选择远程防御塔界面】【升级拆除界面】这两个界面有没有在显示，有则处理这些先
    上面两个没有显示才去下面处理点击空砖块(CLICK_OBJECT)的情况
*///下面是上述思路的一个实现:

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//第一步判断：是否点击近战塔已经显示的【升级拆除按钮】
        for(auto tower_item = MeleeTowerVector.begin();tower_item!=MeleeTowerVector.end();)
        {
            bool erase_unused = true;
            if((*tower_item)->update_remove_is_showed())//扫描到当前塔的【升级拆除界面】是显示状态
            {
                //1.点击了【右边相邻】的的拆除按钮
                if(CLICK_RIGION(ev->x(),ev->y(),
                                (*tower_item)->get_position().x+map_block_size,
                                (*tower_item)->get_position().y,
                                map_block_size,
                                map_block_size
                                ))//若鼠标点击的是这个塔右边相邻一个格子的区域,也就是我设定的【升级或拆除按钮】的图像显示区域
                {   (*tower_item)->hide_update_remote();//这个塔不再显示【升级拆除按钮】
                    Money += (*tower_item)->get_value()*0.7;//拆塔返还70%的塔的价值[退钱！]
                    //更新金钱标签
                    MoneyLabel->setText(QString("%1").arg(Money));
                    MoneyLabel->adjustSize();
                    TowerBase* del = (*tower_item);
                    MeleeTowerVector.erase(tower_item);
                    delete del;//释放空间
                    erase_unused = false;//迭代器已经指向下一个元素了，不要再自增了
                    update();return;//标准动作，结束此次点击事件的处理
                }

                //2.点击了【左边相邻】的的升级按钮
                /////////////////////后续待完善          [########]
                //TODO





                break;
            }

            if(erase_unused) tower_item++;
        }





///[!!!!!!!!]这里后期加上对远程塔数组的扫描(与上面一样的模式)
/// ///phase2统一了近战塔与远程塔，上述不用加了
//第二步判断：是否点击远程塔已经显示的【升级拆除按钮】                                                  [########][********](还没写)[!!!!!!!!]
//TODO

















//第三步判断：是否点击在【选择近战防御塔的框】
        if(melee_tower_selection.MeleeTowerSelection_is_showed())
        {
            //注意这里必须通过【选择防御塔的框】来判断把塔加在什么位置上

            //放置的新防御塔在地图上的像素位置
            location new_tower_position(melee_tower_selection.get_position().x+map_block_size,
                                        melee_tower_selection.get_position().y+map_block_size);

            //1.点击了【正上方】的tower_1的按钮
            if(CLICK_RIGION(ev->x(),ev->y(),
                            melee_tower_selection.get_position().x+map_block_size,
                            melee_tower_selection.get_position().y,
                            map_block_size,
                            map_block_size
                            ))//若鼠标点击的是这个砖块上面相邻一个格子的区域,也就是我设定的【选择近战防御塔界面】的tower_1图像显示区域
            {
                Tower_1* new_tower1 = new Tower_1(new_tower_position,GameTime);
                if(cost_money(new_tower1))//如果买的起，就加入MeleeTowerVector数组
                    MeleeTowerVector.append(new_tower1);
                melee_tower_selection.MeleeTowerSelection_hide();
                update();return;//完成本次点击事件的处理

            }
            //2.点击了【右上方】的tower_2的按钮
            if(CLICK_RIGION(ev->x(),ev->y(),
                            melee_tower_selection.get_position().x+2*map_block_size,
                            melee_tower_selection.get_position().y,
                            map_block_size,
                            map_block_size
                            ))//若鼠标点击的是这个砖块上面相邻一个格子的区域,也就是我设定的【选择近战防御塔界面】的tower_1图像显示区域
            {
                Tower_2* new_tower2 = new Tower_2(new_tower_position,GameTime);
                if(cost_money(new_tower2))//如果买的起，就加入MeleeTowerVector数组
                    MeleeTowerVector.append(new_tower2);
                melee_tower_selection.MeleeTowerSelection_hide();
                update();return;//完成本次点击事件的处理

            }
            //3.点击了【右边相邻】的tower_6的按钮///////////////////////////////////...
            //[########]后续加入不同防御塔的时候这里也要加上按钮方案
            //TODO






        }

//第四步判断：是否点击在【选择远程防御塔的框】
        if(remote_tower_selection.RemoteTowerSelection_is_showed())
        {

            //注意这里必须通过【选择防御塔的框】来判断把塔加在什么位置上

            //放置的新防御塔在地图上的像素位置
            location new_tower_position(remote_tower_selection.get_position().x+map_block_size,
                                        remote_tower_selection.get_position().y+map_block_size);

            //1.点击了【上方】的tower_3的按钮
            if(CLICK_RIGION(ev->x(),ev->y(),
                            remote_tower_selection.get_position().x+map_block_size,
                            remote_tower_selection.get_position().y,
                            map_block_size,
                            map_block_size
                            ))//若鼠标点击的是这个砖块上面相邻一个格子的区域,也就是我设定的【选择远程防御塔界面】的tower_3图像显示区域
            {
                Tower_3* new_tower3 = new Tower_3(new_tower_position,GameTime);
                if(cost_money(new_tower3))//如果买的起，就加入MeleeTowerVector数组
                    MeleeTowerVector.append(new_tower3);///将远程塔加入MeleeTowerVector中
                remote_tower_selection.RemoteTowerSelection_hide();
                update();return;//完成本次点击事件的处理

            }
            //2.点击了【右方】的tower_4的按钮
            if(CLICK_RIGION(ev->x(),ev->y(),
                            remote_tower_selection.get_position().x+2*map_block_size,
                            remote_tower_selection.get_position().y+map_block_size,
                            map_block_size,
                            map_block_size
                            ))//若鼠标点击的是这个砖块右边相邻一个格子的区域,也就是我设定的【选择远程防御塔界面】的tower_4图像显示区域
            {
                Tower_4* new_tower4 = new Tower_4(new_tower_position,GameTime);
                if(cost_money(new_tower4))//如果买的起，就加入MeleeTowerVector数组
                    MeleeTowerVector.append(new_tower4);///将远程塔加入MeleeTowerVector中
                remote_tower_selection.RemoteTowerSelection_hide();
                update();return;//完成本次点击事件的处理

            }
            //3.点击了【左方】的tower_5的按钮
            if(CLICK_RIGION(ev->x(),ev->y(),
                            remote_tower_selection.get_position().x,
                            remote_tower_selection.get_position().y+map_block_size,
                            map_block_size,
                            map_block_size
                            ))//若鼠标点击的是这个砖块上面相邻一个格子的区域,也就是我设定的【选择远程防御塔界面】的tower_5图像显示区域
            {
                Tower_5* new_tower5 = new Tower_5(new_tower_position,GameTime);
                if(cost_money(new_tower5))//如果买的起，就加入MeleeTowerVector数组
                    MeleeTowerVector.append(new_tower5);///将远程塔加入MeleeTowerVector中
                remote_tower_selection.RemoteTowerSelection_hide();
                update();return;//完成本次点击事件的处理

            }
            //4.点击了【下边相邻】的tower_6的按钮///////////////////////////////////...
            //[########]后续加入更多远程防御塔的时候这里也要加上按钮方案
            //TODO






        }
//第五步判断：是否点击在【墙、出生点、要塞、路径】上
        int obj = CLICK_OBJECT(ev->x(),ev->y());//obj为本次点击的砖块物件类型的代号    0 WALL   1 PATH   2 BIRTH   3 TOWER   4 FORTRESS
//        test
//        QString test_output1 = QString("点击的物件代号是%1").arg(obj);
//        qDebug()<<test_output1;

        if(obj!=-1)//如果鼠标点在了有含义的物体砖块上   (如果点击生命金钱label摆放的位置就返回-1，则不作反应
        {
            //判断点击的是什么物件
            switch(obj)
            {
            case WALL://点击的是墙
                {}
                break;
            case BIRTH://点击的是怪物出生点的砖块
                {}
                break;
            case FORTRESS://点击的是堡垒要塞
                {}
                break;
            //目前点击墙/怪物出生点/堡垒要塞是没有加任何操作的，后续可以丰富更多操作在上面的代码段      [########]
            case PATH://点击的是路径
                {
                    //1.如果当前点击的路径上有近战塔
                    for(auto tower_item = MeleeTowerVector.begin();tower_item!=MeleeTowerVector.end();tower_item++)
                    {
                        if(match_tower(ev->x(),ev->y(),(*tower_item)))
                        {   //(1)把所有近战塔的【升级拆除界面】隐藏
                            for(auto temp = MeleeTowerVector.begin();temp!=MeleeTowerVector.end();temp++)
                                (*temp)->hide_update_remote();
                            //(2)把【选择近战防御塔界面】关掉
                            melee_tower_selection.MeleeTowerSelection_hide();
                            ///////////下面待远程防御塔补充
                            //(3)把【选择远程防御塔界面】关掉
                            remote_tower_selection.RemoteTowerSelection_hide();
                            //(4)把所有远程塔的【升级拆除界面】隐藏              [########][********](还没写)[!!!!!!!!]
                            //TODO
                            //phase2:目前远程塔和近战塔全部放在了meleetowerVector中，所以这一步和第(1)步合并了




                            //(4)把当前这个近战塔的【升级拆除界面】显示
                            (*tower_item)->do_show_update_remote();
                            update();return;//标准动作：结束本次鼠标事件处理
                        }
                    }
                    //2.如果没有通过上面的循环结束本次处理，则当前点击的路径上没有塔
                    //(1)把所有近战塔的【升级拆除界面】隐藏
                    for(auto temp = MeleeTowerVector.begin();temp!=MeleeTowerVector.end();temp++)
                        (*temp)->hide_update_remote();
                    //(2)配置【选择近战防御塔界面】的位置
                    location temp = CLICK_BLOCK(ev->x(),ev->y());
                    temp.x-=map_block_size;
                    temp.y-=map_block_size;//因为【选择近战防御塔界面】是3*3的，而CLICK_BLOCK获取的是其正中心的格子的左上角，故还要左移一格再上移一格才是【选择近战防御塔界面】的坐标
                    melee_tower_selection.set_position(temp);
                    //(3)把【选择近战防御塔界面】显示
                    melee_tower_selection.MeleeTowerSelection_show();
                    ///////////下面待远程防御塔补充
                    //(4)把【选择远程防御塔界面】关掉
                    remote_tower_selection.RemoteTowerSelection_hide();
                    //(5)把所有远程塔的【升级拆除界面】隐藏                     [########][********](还没写)[!!!!!!!!]
                    //TODO
                    //phase2:目前远程塔和近战塔全部放在了meleetowerVector中，所以这一步和第(1)步合并了





                    update();return;//结束本次点击事件处理
                }
                break;
            case TOWER://点击的是放远程防御塔的位置(因为近战塔放在了路径上)
                {
                    //1.如果当前点击的位置上有塔
                    for(auto tower_item = MeleeTowerVector.begin();tower_item!=MeleeTowerVector.end();tower_item++)
                    {
                        if(match_tower(ev->x(),ev->y(),(*tower_item)))
                        {   //(1)把所有塔的【升级拆除界面】隐藏
                            for(auto temp = MeleeTowerVector.begin();temp!=MeleeTowerVector.end();temp++)
                                (*temp)->hide_update_remote();
                            //(2)把【选择近战防御塔界面】关掉
                            melee_tower_selection.MeleeTowerSelection_hide();
                            ///////////下面待远程防御塔补充
                            //(3)把【选择远程防御塔界面】关掉
                            remote_tower_selection.RemoteTowerSelection_hide();
                            //(4)把所有远程塔的【升级拆除界面】隐藏              [########][********](还没写)[!!!!!!!!]
                            //TODO
                            //phase2:目前远程塔和近战塔全部放在了meleetowerVector中，所以这一步和第(1)步合并了




                            //(4)把当前这个塔的【升级拆除界面】显示
                            (*tower_item)->do_show_update_remote();
                            update();return;//标准动作：结束本次鼠标事件处理
                        }
                    }
                    //2.如果没有通过上面的循环结束本次处理，则当前点击的位置上没有塔
                    //(1)把所有塔的【升级拆除界面】隐藏
                    for(auto temp = MeleeTowerVector.begin();temp!=MeleeTowerVector.end();temp++)
                        (*temp)->hide_update_remote();
                    //(2)配置【选择近战防御塔界面】的位置
                    location temp1 = CLICK_BLOCK(ev->x(),ev->y());
                    temp1.x-=map_block_size;
                    temp1.y-=map_block_size;//因为【选择近战防御塔界面】是3*3的，而CLICK_BLOCK获取的是其正中心的格子的左上角，故还要左移一格再上移一格才是【选择近战防御塔界面】的坐标
                    remote_tower_selection.set_position(temp1);
                    //(3)把【选择远程防御塔界面】显示
                    qDebug()<<"把【选择远程防御塔界面】显示";
                    remote_tower_selection.RemoteTowerSelection_show();
                    ///////////下面待远程防御塔补充
                    //(4)把【选择近战防御塔界面】关掉
                    melee_tower_selection.MeleeTowerSelection_hide();
                    //(5)把所有远程塔的【升级拆除界面】隐藏                     [########][********](还没写)[!!!!!!!!]
                    //TODO
                    //phase2:目前远程塔和近战塔全部放在了meleetowerVector中，所以这一步和第(1)步合并了





                    update();return;//结束本次点击事件处理
                }/////////////////////////加入对远程防御塔的点击操作在这////////////////////////////////        [########][********](还没写)[!!!!!!!!]
                ///////////////////////////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////////////////
                break;
            default:
                break;

            }
        }
        else//假设鼠标点在了没含义的地方（如生命金钱栏）     就关闭所有已经点开的界面
        {//把所有近战塔、远程塔的【升级拆除界面】隐藏，把【选择近战防御塔界面】【选择远程防御塔界面】隐藏，即全部界面都隐藏

            //(1)把所有近战塔的【升级拆除界面】隐藏
            for(auto temp = MeleeTowerVector.begin();temp!=MeleeTowerVector.end();temp++)
                (*temp)->hide_update_remote();
            //(2)把【选择近战防御塔界面】关掉
            melee_tower_selection.MeleeTowerSelection_hide();
            ///////////下面待远程防御塔补充
            //(3)把【选择远程防御塔界面】关掉
            remote_tower_selection.RemoteTowerSelection_hide();
            //(4)把所有远程塔的【升级拆除界面】隐藏                     [########][********](还没写)[!!!!!!!!]
            //TODO
            //phase2:目前远程塔和近战塔全部放在了meleetowerVector中，所以这一步和第(1)步合并了




        }

    }
}

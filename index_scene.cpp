//显示HTML写的instruction文档


#include "index_scene.h"
#include<QProcess>//用来ping gitee.com判断网络连接状态的
#include<QThread>//用来ping gitee.com判断网络连接状态的
#include<QLabel>//用来显示网络连接不成功的页面
#include<QFont>
index_scene::index_scene(QWidget *parent) :
    QMainWindow(parent)
{
///////////////////////////////////////////////设置页面基本信息///////////////////////////////////////////////////////////////////
    //更改窗口大小
    this->resize(1000,990);
    //设置图标
    setWindowIcon(QIcon(":/res/window_icon.png"));
    //设置标题
    setWindowTitle("基本信息");

    index_HTML = new QWebView(this);






///////////////////////////////////////////////测试网络连接状态///////////////////////////////////////////////////////////////////
    bool network_available = false;
    QString test_network_cmd = "ping gitee.com -n 2 -w 500";
    QString result;
    QProcess * test_network_process = new QProcess();//不要加this
    test_network_process->start(test_network_cmd);//调用ping指令
    test_network_process->waitForFinished();//等待指令执行完毕
    result = test_network_process->readAll();//获取指令执行结果
    if(result.contains(QString("TTL=")))//若包含TTL=字符串就视为连接成功
    {
        //qDebug()<<"连接成功";
        network_available = true;
    }
    else
    {
        //qDebug()<<"连接失败";
        network_available = false;
    }







////////////////////////////////////////情况1：网络在线,把网络获取的html文档显示出来////////////////////////////////////////
    if(network_available == true)
    {
        //方案1.查看本地index文档(弃用)
    //    ///[********]注意查看本地文档时，这里的路径只适用于xj_mao电脑（因为是硬编码的绝对路径）,所以在移植项目到其他设备时一定要修改这里的URL路径成设备本地的html文档
    //    index_HTML->load(QUrl("file:///D:/TowerDefence/index/index.html"));

        //方案2.查看部署在gitee上的远程index文档(需要游戏在联网环境下运行)
        index_HTML->load(QUrl("http://aesopd032.gitee.io/tower-defence-index/"));
        //显示加载的内容
        index_HTML->show();
    }
////////////////////////////////////////情况2：网络离线,显示网络未连接的界面///////////////////////////////////////////////
    else
    {   //qDebug()<<"网络连接失败";
        QLabel *picture1 = new QLabel(this);
        picture1->resize(300,300);
        picture1->setPixmap(QPixmap("://res/network_unavailable.png"));
        picture1->move(350,140);

        QFont title_font("Microsoft YaHei", 30, 75);
        QLabel *title = new QLabel(this);
        title->setText("   呃...网络连接失败。");
        title->adjustSize();
        title->setFont(title_font);
        title->resize(width(),height());
        title->setAlignment(Qt::AlignHCenter);
        title->move(0,460);

        QFont body1_font("Microsoft YaHei", 20, 75);
        QLabel *body1 = new QLabel(this);
        body1->setText("您可以尝试:");
        body1->setFont(body1_font);
        body1->adjustSize();
        body1->move(100,570);

        QFont body2_font("Microsoft YaHei", 15, 25);
        QLabel *body2 = new QLabel(this);
        body2->setText("· 过会儿再重试。\n\n· 检查您的网络连接是否正常。\n\n·如果您部署有网络防火墙，请检查TowerDefence是否已被授权访问网络。");
        body2->setFont(body2_font);
        body2->adjustSize();
        body2->move(100,630);

    }

}

void index_scene::resizeEvent(QResizeEvent *e)//这个函数用途是让QWebView的大小随着整个窗体缩放而变大或者变小
{
    int w, h;
    w = width();
    h = height();
    index_HTML->setGeometry(0, 0, w*0.99, h*0.99);
    QMainWindow::resizeEvent(e);
}

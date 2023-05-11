//显示HTML写的instruction文档

#ifndef INDEX_SCENE_H
#define INDEX_SCENE_H

#include <QMainWindow>
#include<QWebView>

class index_scene : public QMainWindow
{
    Q_OBJECT
public:
    explicit index_scene(QWidget *parent = 0);
    QWebView * index_HTML;//用于显示index
    virtual void resizeEvent(QResizeEvent *);//用于使显示内容适配窗口大小
signals:

public slots:

};

#endif // INDEX_SCENE_H

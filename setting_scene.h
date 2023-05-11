#ifndef SETTING_SCENE_H
#define SETTING_SCENE_H

#include <QWidget>

namespace Ui {
class setting_scene;
}

class setting_scene : public QWidget
{
    Q_OBJECT

public:
    explicit setting_scene(QWidget *parent = 0);
    ~setting_scene();

private:
    Ui::setting_scene *ui;

signals:
    //用于设置音量的信号函数
    void volume_setting_signal(int i);
public slots:

};

#endif // SETTING_SCENE_H

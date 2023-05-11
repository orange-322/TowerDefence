#-------------------------------------------------
#
# Project created by QtCreator 2021-10-02T19:14:38
#
#-------------------------------------------------

QT       += core gui multimedia webkitwidgets network webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TowerDefence
TEMPLATE = app


SOURCES += main.cpp\
        mainscene.cpp \
    mypushbutton.cpp \
    choosestatescene.cpp \
    gamewindow.cpp \
    enemybase.cpp \
    enemy_1.cpp \
    towerbase.cpp \
    tower_1.cpp \
    meleetowerselection.cpp \
    tower_2.cpp \
    enemy_2.cpp \
    remotetowerselection.cpp \
    tower_3.cpp \
    tower_4.cpp \
    enemy_3.cpp \
    setting_scene.cpp \
    index_scene.cpp \
    enemy_4.cpp \
    towerbullet.cpp \
    tower_5.cpp

HEADERS  += mainscene.h \
    mypushbutton.h \
    choosestatescene.h \
    gamewindow.h \
    enemybase.h \
    global_structure.h \
    enemy_1.h \
    towerbase.h \
    tower_1.h \
    meleetowerselection.h \
    map_and_path.h \
    tower_2.h \
    enemy_2.h \
    remotetowerselection.h \
    tower_3.h \
    tower_4.h \
    enemy_3.h \
    setting_scene.h \
    index_scene.h \
    enemy_4.h \
    towerbullet.h \
    tower_5.h

FORMS    += mainscene.ui \
    setting_scene.ui



RESOURCES += \
    res.qrc


CONFIG += c++11

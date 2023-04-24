QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia network concurrent sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#程序版本
VERSION  = 1.0
#程序图标
RC_ICONS = icon/MusicPlayer.ico
#产品名称
QMAKE_TARGET_PRODUCT = MusicPlayer
#版权所有
QMAKE_TARGET_COPYRIGHT = hao
#文件说明
QMAKE_TARGET_DESCRIPTION = HaoMusic

#禁用qdebug打印输出
#DEFINES += QT_NO_DEBUG_OUTPUT

#关闭编译警告提示 眼不见为净
#CONFIG      += warn_off

#指定编译生成的文件到temp目录 分门别类存储
#MOC_DIR     = temp/moc
#RCC_DIR     = temp/rcc
#UI_DIR      = temp/ui
#OBJECTS_DIR = temp/obj

#指定编译生成的可执行文件到bin目录
#DESTDIR     = bin

SOURCES += \
    addsonglistpage.cpp \
    customitem.cpp \
    customslider.cpp \
    gaussianblur.cpp \
    iconlist.cpp \
    lrcwidget.cpp \
    main.cpp \
    haomusic.cpp \
    music.cpp \
    musicdb.cpp \
    mybottombar.cpp \
    myhttp.cpp \
    mylabel.cpp \
    mylineedit.cpp \
    mylistwidget.cpp \
    mymediaplaylist.cpp \
    shadowwidget.cpp \
    threaddownloader.cpp

HEADERS += \
    addsonglistpage.h \
    customslider.h \
    customitem.h \
    gaussianblur.h \
    haomusic.h \
    iconlist.h \
    lrcwidget.h \
    music.h \
    musicdb.h \
    mybottombar.h \
    myhttp.h \
    mylabel.h \
    mylineedit.h \
    mylistwidget.h \
    mymediaplaylist.h \
    shadowwidget.h \
    threaddownloader.h

FORMS += \
    addsonglistpage.ui \
    customitem.ui \
    haomusic.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS += -Wno-unused-parameter

RESOURCES += \
    res.qrc


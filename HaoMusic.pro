QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customitem.cpp \
    customslider.cpp \
    main.cpp \
    haomusic.cpp \
    music.cpp \
    mybottombar.cpp \
    myhttp.cpp \
    mylineedit.cpp

HEADERS += \
    customslider.h \
    customitem.h \
    haomusic.h \
    music.h \
    mybottombar.h \
    myhttp.h \
    mylineedit.h

FORMS += \
    customitem.ui \
    haomusic.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_CXXFLAGS += -Wno-unused-parameter

RESOURCES += \
    res.qrc

#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QCoreApplication>

class CustomSlider : public QSlider
{
    Q_OBJECT
    public:
        CustomSlider(QWidget *parent = nullptr);
    protected:
        void mousePressEvent(QMouseEvent *ev);//重写QSlider的mousePressEvent事件
        void mouseReleaseEvent(QMouseEvent *ev);
    signals:
        void customSliderClicked();//自定义的鼠标单击信号，用于捕获并处理
};

#endif // CUSTOMSLIDER_H

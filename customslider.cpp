#include "customslider.h"

CustomSlider::CustomSlider(QWidget *parent)
    : QSlider(parent)
{

}

// 鼠标点击事件
void CustomSlider::mousePressEvent(QMouseEvent *ev)
{
    if (Qt::LeftButton == ev->button()) {
        //注意应先调用父类的鼠标点击处理事件，这样可以不影响拖动的情况
        QSlider::mousePressEvent(ev);
        //获取鼠标的位置，这里并不能直接从ev中取值（因为如果是拖动的话，鼠标开始点击的位置没有意义了）
        double pos = ev->x() / (double)width();
        //让进度条直接蹦过来
        setValue(pos * (maximum() - minimum()) + minimum());
        //发送自定义的鼠标单击信号
        emit customSliderClicked();
    }

}


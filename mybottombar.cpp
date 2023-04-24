#include "mybottombar.h"
#include <QMouseEvent>

MyBottomBar::MyBottomBar(QWidget *parent)
    : QWidget{parent}
{

}

void MyBottomBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button()) {
        emit clicked();
    }
    QWidget::mouseReleaseEvent(event);
}

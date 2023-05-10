#include "searchtipslist.h"

#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QPropertyAnimation>
#include <QDebug>

SearchTipsList::SearchTipsList(QWidget *parent)
    : QListWidget{parent}
{
    this->setObjectName("searchTips");
    this->setMaximumHeight(600);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 阴影
    this->setContentsMargins(11, 11, 11, 11);
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    //窗口添加对应的阴影效果
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(QColor(150,150,150));
    shadow_effect->setBlurRadius(6);
    this->setGraphicsEffect(shadow_effect);
}

void SearchTipsList::updateSize()
{
    this->setMaximumHeight(this->sizeHintForRow(0) * 8.5 + 2 * this->frameWidth());
    this->resize(this->sizeHint());
}

void SearchTipsList::showTips()
{
    this->resize(this->width(), 0);
    this->show();
    animation(true);
}

void SearchTipsList::hideTips()
{
    animation(false);
}

void SearchTipsList::paintEvent(QPaintEvent *e)
{
//    qDebug() << __FILE__ << __LINE__ << this->height();
    QListWidget::paintEvent(e);
}

QSize SearchTipsList::sizeHint()
{
    return QSize(this->sizeHintForColumn(0) + 2 * this->frameWidth(), this->sizeHintForRow(0) * this->count() + 2 * this->frameWidth());
}

void SearchTipsList::animation(bool show)
{
    qDebug() << __FILE__ << __LINE__ << show;
    QPropertyAnimation* ani = new QPropertyAnimation(this, "size");
    ani->setStartValue(QSize(this->width(), 0));    // 高度为0
    ani->setEndValue(sizeHint());                   // 高度正常
    ani->setEasingCurve(QEasingCurve::OutQuad);     // 变化曲线
    ani->setDuration(300);                          // 动画时长
    if (show) { // 展开动画
        ani->setDirection(QAbstractAnimation::Forward);
    } else {    // 隐藏动画
        ani->setDirection(QAbstractAnimation::Backward);
        connect(ani, &QPropertyAnimation::finished, this, &SearchTipsList::hide);
    }
    ani->start(QAbstractAnimation::DeleteWhenStopped);
}



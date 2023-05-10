#include "switchanimation.h"


SwitchAnimation::SwitchAnimation(QWidget *parent)
    : QWidget{parent}
{

}

void SwitchAnimation::slideAnimation(QWidget *parent, QWidget *slideWidget, int direction)
{
    // 初始化label，用来存储界面的截图
    if (!m_leftLabel)
        m_leftLabel = new QLabel(parent);
    m_leftLabel->setStyleSheet("background-color: white");
    m_leftLabel->resize(slideWidget->size());
    m_leftLabel->raise();

    // 设置界面动画
    if (!anim) {
        anim = new QPropertyAnimation(m_leftLabel, "geometry", this);
        connect(anim, &QPropertyAnimation::finished, [=] {
            m_leftLabel->hide();
        });
    }
    anim->setDuration(animTime);
    anim->setEndValue(slideWidget->geometry());
    anim->setEasingCurve(QEasingCurve::OutCubic);
    // 确定动画方向
    switch (direction) {
    case AnimDirection::Forward:
    case AnimDirection::Backward:
        anim->setStartValue(QRect(slideWidget->width(), 0, slideWidget->width(), slideWidget->height()));
        break;
    case AnimDirection::Up:
    case AnimDirection::Down:
        anim->setStartValue(QRect(0, slideWidget->height(), slideWidget->width(), slideWidget->height()));
        break;
    }

    // 用label存储两个界面的截图
    m_leftLabel->setPixmap(slideWidget->grab(slideWidget->rect()).scaled(m_leftLabel->size()));
//    m_leftLabel->setPixmap(QPixmap::grabWidget(slideWidget));
    // 显示label
    m_leftLabel->show();
    // 提升label
    m_leftLabel->raise();

    switch (direction) {
    case AnimDirection::Forward:
    case AnimDirection::Up:
        anim->setDirection(QAbstractAnimation::Forward);
        break;
    case AnimDirection::Down:
    case AnimDirection::Backward:
        anim->setDirection(QAbstractAnimation::Backward);
        break;
    }
    // 播放动画
    anim->start();
}

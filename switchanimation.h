#ifndef SWITCHANIMATION_H
#define SWITCHANIMATION_H

#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class SwitchAnimation : public QWidget
{
public:
    enum AnimDirection {
        Forward,
        Backward,
        Up,
        Down
    };
    SwitchAnimation(QWidget *parent);
    /*
     * @function: 实现两个窗口的平移切换动画
     * @param: parent 两个窗口的父窗口，一般是QStackWidget
     * @param: leftWidget 左窗口
     * @param: rigjtWidget 右窗口
     * @param: direction 平移的方向
     */
    void slideAnimation(QWidget *parent, QWidget * slideWidget, int direction = AnimDirection::Up);
protected:
    int animTime = 400;
private:
    // 界面平移切换动画
    QLabel *m_leftLabel = nullptr;
    QPropertyAnimation *anim = nullptr;
};

#endif // SWITCHANIMATION_H

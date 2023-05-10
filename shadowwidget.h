#ifndef SHADOWWIDGET_H
#define SHADOWWIDGET_H

#include <QLabel>
#include <QParallelAnimationGroup>

class ShadowWidget : public QWidget
{
    Q_OBJECT
public:
    enum AnimDirection {
        Forward,
        Backward,
        Up,
        Down
    };
    explicit ShadowWidget(QWidget *parent = nullptr);
protected:
    // 重写绘图事件，实现圆角阴影窗口
    void paintEvent(QPaintEvent *event);
    // 重写鼠标按下、移动、释放事件，实现窗口跟随鼠标拖动
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    /*
     * @function: 实现两个窗口的平移切换动画
     * @param: parent 两个窗口的父窗口，一般是QStackWidget
     * @param: leftWidget 左窗口
     * @param: rigjtWidget 右窗口
     * @param: direction 平移的方向
     */
    void siwtchingAnimation(QWidget *parent, QWidget * leftWidget, QWidget *rightWidget, int direction = AnimDirection::Forward);

protected:
    int animTime = 600;
private:
    // 初始化窗口
    void initWidget();

private:
    int m_alphaValue = 50;      //透明度
    int m_radius     = 6;       //默认窗口的圆角半径
    QVector<int> m_blurValue;   //阴影渐变的颜色
    QColor m_shadowColor = QColor("#65666e");   //阴影的颜色
    QColor m_backgroundColor = Qt::white;       //窗口的背景颜色

    //能够移动窗口的属性
    bool m_lefted = false;
    QPoint m_leftPoint;

    // 界面平移切换动画
    QLabel *m_leftLabel = nullptr;
    QLabel *m_rightLabel = nullptr;
    QParallelAnimationGroup *m_animGroup = nullptr;
};

#endif // SHADOWWIDGET_H

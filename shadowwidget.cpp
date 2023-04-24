#include "shadowwidget.h"
#include <qmath.h>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QTimer>
#include <QPropertyAnimation>

TipLabel::TipLabel(QWidget *parent)
    : QLabel{parent}
{
    // 设置样式
    this->setStyleSheet("QLabel{color:blue;background:transparent;}");
    // 垂直、水平中心对齐
    this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    // 与父窗口宽度相同
    this->setFixedWidth(parent->width());
    // 设置文字可换行
    this->setWordWrap(true);
}

void TipLabel::showTip(QString tip)
{
    // 显示提示，两秒后消失
    this->setText(tip);
    this->show();
    QTimer::singleShot(2000, this, &TipLabel::hide);
}

ShadowWidget::ShadowWidget(QWidget *parent)
    : QWidget{parent}
{
    initWidget();
}

// 初始化窗口
void ShadowWidget::initWidget()
{
    //TODO: 设置窗口无边框/背景透明属性
    setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);//无边框的窗口
    setAttribute(Qt::WA_TranslucentBackground);//实现圆角的无边框窗口必须设置的属性

    //TODO: 初始化阴影的渐变值
    qreal factor = m_alphaValue / m_radius;
    for (int i = 0; i < m_radius; i++)
    {
        int level = (int)(m_alphaValue - qSqrt(i) * factor + 0.5);
        m_blurValue.push_back(qMax<int>(0, level - 2));
    }
}

// 显示提示信息
void ShadowWidget::showTip(QString tip, int x, int y, QWidget *parent)
{
    // 初始化tiplabel
    if (nullptr == m_tipLabel) {
        // 如果没有传递父窗口，则父窗口为当前类对象
        if (!parent)
            parent = this;
        m_tipLabel = new TipLabel(parent);
    }
    // 以父窗口为基准QPoint(0, 0)，移动到QPoint(x, y)
    m_tipLabel->move(x, y);
    // 显示提示信息
    m_tipLabel->showTip(tip);
}

// 界面平移切换动画
void ShadowWidget::siwtchingAnimation(QWidget *parent ,QWidget *leftWidget, QWidget *rightWidget, int direction)
{
    // 初始化动画
    if (m_animGroup == nullptr) {
        // 初始化两个label，用来存储两个界面的截图
        m_leftLabel = new QLabel(parent);
        m_leftLabel->resize(leftWidget->size());
        m_leftLabel->raise();

        m_rightLabel = new QLabel(parent);
        m_rightLabel->resize(rightWidget->size());
        m_rightLabel->raise();
        // 设置左界面动画
        QPropertyAnimation *animLeft = new QPropertyAnimation(m_leftLabel, "geometry");
        animLeft->setDuration(animTime);
        animLeft->setStartValue(leftWidget->geometry());
        animLeft->setEndValue(QRect(-(leftWidget->width()), 0, leftWidget->width(), leftWidget->height()));
        animLeft->setEasingCurve(QEasingCurve::OutCubic);
        // 设置右界面动画
        QPropertyAnimation *animRight = new QPropertyAnimation(m_rightLabel, "geometry");
        animRight->setDuration(animTime);
        animRight->setStartValue(QRect(rightWidget->width(), 0, 0, rightWidget->height()));
        animRight->setEndValue(rightWidget->geometry());
        animRight->setEasingCurve(QEasingCurve::OutCubic);
        // 将两个界面动画添加到并行动画组
        m_animGroup = new QParallelAnimationGroup(this);
        m_animGroup->addAnimation(animLeft);
        m_animGroup->addAnimation(animRight);
    }

    // 用两个label存储两个界面的截图
    m_leftLabel->setPixmap(leftWidget->grab());
    m_rightLabel->setPixmap(rightWidget->grab());
    // 显示label
    m_leftLabel->show();
    m_rightLabel->show();
    // 提升label
    m_leftLabel->raise();
    m_rightLabel->raise();

    // 选择动画的方向
    if (AnimDirection::Forward == direction) {
        m_animGroup->setDirection(QAbstractAnimation::Forward);
    }
    if (AnimDirection::Backward == direction) {
        m_animGroup->setDirection(QAbstractAnimation::Backward);
    }

    // 播放动画，动画结束隐藏label
    m_animGroup->start(QAbstractAnimation::KeepWhenStopped);
    connect(m_animGroup, &QParallelAnimationGroup::finished, [=] {
        m_leftLabel->hide();
        m_rightLabel->hide();
    });
}

// 绘图事件
void ShadowWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    //TODO: 先填充背景，原来是透明的，用背景颜色进行填充
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    path.addRoundedRect(m_radius, m_radius, width() - m_radius * 2,
                        height() - m_radius * 2, m_radius, m_radius);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillPath(path, QBrush(m_backgroundColor));

    //TODO: 再绘制圆角矩形path，利用渐变的颜色。
    auto color = m_shadowColor;
    for (int i = 0; i < m_radius; i++)
    {
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);  // 缠绕填充
        path.addRoundedRect(m_radius - i, m_radius - i, width() - (m_radius - i) * 2,
                            height() - (m_radius - i) * 2, m_radius, m_radius);

        int alpha = 0;
        if (i < m_blurValue.size())
        {
            alpha = m_blurValue[i];
        }
        color.setAlpha(alpha);
        painter.setPen(color);
        painter.drawPath(path);
    }

}

// 鼠标移动事件
void ShadowWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_lefted)
    {
        //TODO: 鼠标移动时计算 （当前鼠标的位置- 按下鼠标位置的差= 窗口移动的距离)
        QPoint offset = event->pos() - m_leftPoint;
        move(offset + pos());
    }
}

// 鼠标按下事件
void ShadowWidget::mousePressEvent(QMouseEvent *event)
{
    this->setFocus(Qt::MouseFocusReason);
    if (event->button() == Qt::LeftButton)
    {
        //TODO: 记录鼠标按下时的位置(QPoint)
        m_lefted = true;
        m_leftPoint = event->pos();
    }
}

// 鼠标释放事件
void ShadowWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //TODO: 鼠标释放时重置下记录的数据
        m_lefted = false;
        m_leftPoint.setX(0);
        m_leftPoint.setY(0);
    }
}



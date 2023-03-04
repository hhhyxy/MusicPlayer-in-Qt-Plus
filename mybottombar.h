#ifndef MYBOTTOMBAR_H
#define MYBOTTOMBAR_H

#include <QWidget>

/*
 * 主界面底部控制栏
 */
class MyBottomBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyBottomBar(QWidget *parent = nullptr);
signals:
    // 鼠标点击信号
    void clicked();

protected:
    // 鼠标点击事件
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // MYBOTTOMBAR_H

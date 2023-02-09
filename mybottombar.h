#ifndef MYBOTTOMBAR_H
#define MYBOTTOMBAR_H

#include <QWidget>

class MyBottomBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyBottomBar(QWidget *parent = nullptr);
signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *event);
signals:

};

#endif // MYBOTTOMBAR_H

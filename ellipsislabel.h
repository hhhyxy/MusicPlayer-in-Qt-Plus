#ifndef ELLIPSISLABEL_H
#define ELLIPSISLABEL_H

#include <QLabel>

class EllipsisLabel : public QLabel
{
    Q_OBJECT
public:
    explicit EllipsisLabel(QWidget *parent = nullptr);
    void setEllipsisText(QString text);
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // ELLIPSISLABEL_H

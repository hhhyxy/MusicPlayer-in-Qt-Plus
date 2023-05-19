#ifndef COVERITEM_H
#define COVERITEM_H

#include <QWidget>
#include "mylabel.h"
#include "ellipsislabel.h"
namespace Ui {
class CoverItem;
}

class CoverItem : public QWidget
{
    Q_OBJECT

public:
    explicit CoverItem(QWidget *parent = nullptr);
    ~CoverItem();
    void setCover(QString url);
    void setCoverSize(QSize size);
    void setName(QString name);
    void setMargin(int mar);
    void normal();
    void zoomOut();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
private:
    Ui::CoverItem *ui;
    int margin;
    bool in = false;
};

#endif // COVERITEM_H

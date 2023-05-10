#ifndef SEARCHTIPSLIST_H
#define SEARCHTIPSLIST_H

#include <QListWidget>

class SearchTipsList : public QListWidget
{
    Q_OBJECT
public:
    explicit SearchTipsList(QWidget *parent = nullptr);
    void updateSize();
    void showTips();
    void hideTips();
protected:
    void paintEvent(QPaintEvent *e);
protected:
    QSize sizeHint();
private:
    void animation(bool show = true);
};

#endif // SEARCHTIPSLIST_H

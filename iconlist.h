#ifndef ICONLIST_H
#define ICONLIST_H

#include <QListWidget>

class IconList : public QListWidget
{
    Q_OBJECT
public:
    explicit IconList(QWidget *parent = nullptr);
    void init();
    void addIcons(QMap<int, QString> map);
    void addIcon(int id, QString name);
protected:
    void paintEvent(QPaintEvent *e);
private:
    int iconSize;
    int wordHight;

};

#endif // ICONLIST_H

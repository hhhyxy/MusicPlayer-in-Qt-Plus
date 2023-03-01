#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include "music.h"
#include "customitem.h"
#include <QListWidget>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = nullptr);
    // 添加item
    CustomItem * addCustomItem(Music music);
    QList<CustomItem *> addCustomItems(QList<Music> musicList);
signals:
    void customItemDoubleClicked(CustomItem *item);
};

#endif // MYLISTWIDGET_H

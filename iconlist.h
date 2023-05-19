#ifndef ICONLIST_H
#define ICONLIST_H

#include "coveritem.h"
#include <musiclist.h>
#include <QListWidget>

class IconList : public QListWidget
{
    Q_OBJECT
public:
    explicit IconList(QWidget *parent = nullptr);
    void init();
    void addIcons(QList<MusicList> lists);
    void addIcon(MusicList& list);
signals:
    void create();
protected:

    void resizeEvent(QResizeEvent *e);
private:
    int iconSize;
    int wordHight;
    int margin;
private slots:
    void dropSongsList(QListWidgetItem *item);
    void renameSongsList(QListWidgetItem *item);
    void creatSongsList();
    void onCustomContextMenuRequested(const QPoint &pos);
};

#endif // ICONLIST_H

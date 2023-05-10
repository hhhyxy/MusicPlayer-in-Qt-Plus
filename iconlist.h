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
signals:
    void create();
protected:
    void paintEvent(QPaintEvent *e);
private:
    int iconSize;
    int wordHight;
private slots:
    void dropSongsList(QListWidgetItem *item);
    void renameSongsList(QListWidgetItem *item);
    void creatSongsList();
    void onCustomContextMenuRequested(const QPoint &pos);
};

#endif // ICONLIST_H

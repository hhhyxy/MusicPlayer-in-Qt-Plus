#ifndef MUSICDB_H
#define MUSICDB_H

#include "music.h"
#include "mylistwidget.h"
#include <QGlobalStatic>
#include <QSqlDatabase>


class MusicDB
{
public:
    MusicDB();
    static MusicDB* instance();
    void open();

    void insert(Music &music, int l_id = MyListWidget::FAVORITE);
    QList<Music> query(int l_id);
    bool queryOne(int id);
    void remove(int id, int l_id);

    int insertList(QString& listName);
    QMap<int, QString> queryList();
    void removeList(int l_id);
    void close();
private:
    QSqlDatabase m_db;
};

#endif // MUSICDB_H

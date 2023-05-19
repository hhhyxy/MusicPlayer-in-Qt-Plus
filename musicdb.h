#ifndef MUSICDB_H
#define MUSICDB_H

#include "music.h"
#include "musiclist.h"
#include "mylistwidget.h"

#include <QGlobalStatic>
#include <QSqlDatabase>


class MusicDB
{
public:
    MusicDB();
    static MusicDB* instance();
    void open();
    bool exec(const QString& sql);

    void insert(Music &music, int l_id = MyListWidget::FAVORITE);
    QList<Music> query(int l_id);
    bool queryOne(int id);
    void remove(int id, int l_id);

    int insertList(const MusicList& list);
    QList<MusicList> queryList();
    void updateCover(int id, const QString& cover);
    void updateIntro(int id, const QString& intro);
    void removeList(int l_id);
    void close();
private:
    QSqlDatabase m_db;
};

#endif // MUSICDB_H

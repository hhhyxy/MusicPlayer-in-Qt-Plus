#ifndef MUSICDB_H
#define MUSICDB_H

#include "music.h"
#include <QSqlDatabase>

class MusicDB
{
public:
    enum ListType {
        SEARCHRESULT,
        FAVORITE,
        SONGLIST,
        LOCAL,
        RECENTLY,
    };
    MusicDB();
    void open();
    void insert(Music &music, int listType = ListType::FAVORITE, int index = 0);
    QList<Music> query(int listType, int index = 0);
    void remove(int id, int listType, int index = 0);
    void close();
private:
    QSqlDatabase m_db;
};

#endif // MUSICDB_H

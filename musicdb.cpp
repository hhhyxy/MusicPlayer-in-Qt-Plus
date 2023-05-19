#include "musicdb.h"
#include <QDebug>
#include <QMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>


Q_GLOBAL_STATIC(MusicDB, db)

MusicDB::MusicDB()
{
}

MusicDB *MusicDB::instance()
{
    return db();
}

// 连接数据库
void MusicDB::open()
{
    //"qt_sql_default_connection"
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName("Music.db");
    }
    if (!m_db.open()) {
        qDebug() << __FILE__ << __LINE__ << "Error: Failed to connect database." << m_db.lastError();
    }
    QSqlQuery query(m_db);
    qDebug() << __FILE__ << __LINE__ << "Succeed to connect database." ;

    QString create_sql = "CREATE TABLE IF NOT EXISTS song ("
                         "l_id integer NOT NULL,"
                         "id integer NOT NULL,"
                         "name TEXT NOT NULL,"
                         "author TEXT NOT NULL,"
                         "album TEXT NOT NULL,"
                         "picurl TEXT NOT NULL,"
                         "duration integer NOT NULL"
                       ");";
    query.prepare(create_sql);
    if(!query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "Error: Fail to create table." << query.lastError();
    }
    query.clear();

    QString isTableExist = "select count(*) from sqlite_master where type = 'table' and name = 'musiclist';";
    if(!query.exec(isTableExist)) {
        qDebug() << __FILE__ << __LINE__ << "Error: Fail to query." << query.lastError();
    }
    query.next();
    // 表不存在，创建list表
    if(query.value(0).toInt() == 0) {
        create_sql = "CREATE TABLE musiclist ( "
                     "l_id integer NOT NULL PRIMARY KEY AUTOINCREMENT, "
                     "l_name TEXT NOT NULL, "
                     "l_cover TEXT, "
                     "l_intro TEXT"
                     ");";
        query.prepare(create_sql);
        if(!query.exec()) {
            qDebug() << __FILE__ << __LINE__ << "Error: Fail to create." << query.lastError();
        }
        query.finish();
        MusicList list("本地音乐");
        insertList(list);

        list.setName("历史音乐");
        insertList(list);

        list.setName("我喜欢的音乐");
        insertList(list);

        list.setName("默认歌单");
        insertList(list);
    } else {
        qDebug() << __FILE__ << __LINE__ << "table musiclist existed";
    }
}

// 插入音乐
void MusicDB::insert(Music &music, int l_id)
{
    QSqlQuery song_query(m_db);
    QString insert_sql = "INSERT INTO song (l_id, id, name, author, album, picurl, duration) "
                         "VALUES (:l_id, :id, :name, :author, :album, :picurl, :duration);";
    song_query.prepare(insert_sql);
    song_query.bindValue(":l_id", QString::number(l_id));
    song_query.bindValue(":id", QString::number(music.getId()));
    song_query.bindValue(":name", music.getSongName());
    song_query.bindValue(":author", music.getAuthor());
    song_query.bindValue(":album", music.getAlbumName());
    song_query.bindValue(":picurl", music.albumPicUrl());
    song_query.bindValue(":duration", QString::number(music.songDuration()));
    if(!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "insert error: "<<song_query.lastError();
    }
    else {
        qDebug() << __FILE__ << __LINE__ << "insert success!";
    }
}

// 查询指定歌单的歌曲
QList<Music> MusicDB::query(int l_id)
{
    QSqlQuery song_query(m_db);
    QString querySql = "SELECT id, name, author, album, picurl, duration FROM song WHERE l_id = :l_id;";
    song_query.prepare(querySql);
    song_query.bindValue(":l_id", QString::number(l_id));
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "query error: "<<song_query.lastError();
    }
    QList<Music> musicList;
    while (song_query.next()) {
        int id = song_query.value(0).toInt();
        QString name = song_query.value(1).toString();
        QString author = song_query.value(2).toString();
        QString album = song_query.value(3).toString();
        QString picurl = song_query.value(4).toString();
        int duration = song_query.value(5).toInt();
        Music music(id, name, author, album, picurl, duration);
        musicList.push_front(music);
    }
    return musicList;
}

bool MusicDB::queryOne(int id)
{
    QSqlQuery song_query(m_db);
    QString querySql = "SELECT count(*) FROM song WHERE id = :id;";
    song_query.prepare(querySql);
    song_query.bindValue(":id", QString::number(id));
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "query error: "<<song_query.lastError();
    }
    song_query.next();
    if (song_query.value(0).toInt() == 0) {
        return false;
    }
    return true;
}

// 删除音乐
void MusicDB::remove(int id, int l_id)
{
    QSqlQuery song_query(m_db);
    QString querySql = "DELETE FROM song WHERE l_id = :l_id and id = :id;";

    song_query.prepare(querySql);
    song_query.bindValue(":l_id", QString::number(l_id));
    song_query.bindValue(":id", QString::number(id));
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "delete error: " << song_query.lastError();
    } else {
        qDebug() << __FILE__ << __LINE__ << "delete success";
    }
}

// 增添新的音乐列表
int MusicDB::insertList(const MusicList& list)
{
    // 插入音乐列表
    QSqlQuery song_query(m_db);
    QString insert_sql = "INSERT INTO musiclist (l_name, l_cover, l_intro) "
                         "VALUES (:name, :cover, :intro);";
    song_query.prepare(insert_sql);
    song_query.bindValue(":name", list.name());
    song_query.bindValue(":cover", list.cover());
    song_query.bindValue(":intro", list.intro());
    if(!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "insert musiclist error: " << song_query.lastError();
    }
    else {
        qDebug() << __FILE__ << __LINE__ << "insert musiclist success!";
    }
    // 获取自增id
    QString sql = "SELECT last_insert_rowid() FROM musiclist";
    song_query.prepare(sql);
    if(!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "get id error: "<<song_query.lastError();
    }
    else {
        qDebug() << __FILE__ << __LINE__ << "get id success!";
    }
    song_query.next();
    int id = song_query.value(0).toInt();
    return id;
}

// 查询所有的音乐列表
QList<MusicList> MusicDB::queryList()
{
    QSqlQuery song_query(m_db);
    QString querySql = "SELECT l_id, l_name, l_cover, l_intro FROM musiclist WHERE l_id >= %1";
    song_query.prepare(querySql.arg(MyListWidget::FAVORITE));
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "query error: "<<song_query.lastError();
    }

    QList<MusicList> songList;
    while (song_query.next()) {
        int id = song_query.value(0).toInt();
        QString name = song_query.value(1).toString();
        QString cover = song_query.value(2).toString();
        QString intro = song_query.value(3).toString();
        songList.push_back(MusicList(id, name, cover, intro));
    }
    return songList;
}

void MusicDB::updateCover(int id, const QString &cover)
{
    QSqlQuery song_query(m_db);
    QString querySql = "UPDATE musiclist SET l_cover = :cover WHERE l_id = :id";
    song_query.prepare(querySql);
    song_query.bindValue(":cover", cover);
    song_query.bindValue(":id", id);
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "update error: "<<song_query.lastError();
    }
}

void MusicDB::updateIntro(int id, const QString &intro)
{
    QSqlQuery song_query(m_db);
    QString querySql = "UPDATE musiclist SET l_intro = :intro WHERE l_id = :id";
    song_query.prepare(querySql);
    song_query.bindValue(":intro", intro);
    song_query.bindValue(":id", id);
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "update error: " << song_query.lastError();
    }
}

// 删除音乐列表
void MusicDB::removeList(int l_id)
{
    QSqlQuery song_query(m_db);
    QString querySql = "DELETE FROM musiclist WHERE l_id = :l_id";

    song_query.prepare(querySql);
    song_query.bindValue(":l_id", QString::number(l_id));
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "delete error: "<<song_query.lastError();
    } else {
        qDebug() << __FILE__ << __LINE__ << "delete suscceed: ";
    }
    querySql = "DELETE FROM song WHERE l_id = :l_id";
    song_query.prepare(querySql);
    song_query.bindValue(":l_id", QString::number(l_id));
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "delete error: "<<song_query.lastError();
    } else {
        qDebug() << __FILE__ << __LINE__ << "delete suscceed: ";
    }
}

// 关闭数据库连接
void MusicDB::close()
{
    m_db.close();
    qDebug() << __FILE__ << __LINE__ << "database closed";
}

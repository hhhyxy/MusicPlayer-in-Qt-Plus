﻿#include "musicdb.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

MusicDB::MusicDB()
{
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
    QSqlQuery song_query(m_db);
    qDebug() << __FILE__ << __LINE__ << "Succeed to connect database." ;
    QString isTableExist = QString("select count(*) from sqlite_master where type='table' and name='%1'").arg("music");
    song_query.exec(isTableExist);
    song_query.next();
    //表不存在，创建music表
    if(song_query.value(0).toInt() == 0) {
        qDebug()<< __FILE__ << __LINE__ << "table no exist";
        QString create_sql = "CREATE TABLE music ("
                             "list_type integer NOT NULL,"
                             "idx integer NOT NULL,"
                             "id integer NOT NULL,"
                             "name TEXT NOT NULL,"
                             "author TEXT NOT NULL,"
                             "album TEXT NOT NULL,"
                             "picurl TEXT NOT NULL,"
                             "duration integer NOT NULL,"
                             "PRIMARY KEY ('list_type', 'idx', 'id')"
                           ");";
        song_query.prepare(create_sql);
        if(!song_query.exec()) {
            qDebug() << __FILE__ << __LINE__ << "Error: Fail to create table." << song_query.lastError();
        } else {
            qDebug() << __FILE__ << __LINE__ << "music Table created!";
        }
    }
    song_query.clear();
}

// 插入音乐
void MusicDB::insert(Music &music, int listType, int index)
{
    QSqlQuery song_query(m_db);
    QString insert_sql = "INSERT INTO music (list_type, idx, id, name, author, album, picurl, duration) "
                         "VALUES (:list_type, :idx, :id, :name, :author, :album, :picurl, :duration);";
    song_query.prepare(insert_sql);
    song_query.bindValue(":list_type", QString::number(listType));
    song_query.bindValue(":idx", QString::number(index));
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
    song_query.clear();
}

// 查询歌单
QList<Music> MusicDB::query(int listType, int index)
{
    QSqlQuery song_query(m_db);
    QString querySql = "SELECT id, name, author, album, picurl, duration FROM music WHERE list_type = :list_type and idx = :idx;";
    song_query.prepare(querySql);
    song_query.bindValue(":list_type", QString::number(listType));
    song_query.bindValue(":idx", QString::number(index));
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
        qDebug()<< __FILE__ << __LINE__ << name << author << album;
        Music music(id, name, author, album, picurl, duration);
        musicList.push_front(music);
    }
    song_query.clear();
    return musicList;
}

// 删除音乐
void MusicDB::remove(int id, int listType, int index)
{
    QSqlQuery song_query(m_db);
    QString querySql = "DELETE FROM music WHERE list_type = :list_type and idx = :idx and id = :id;";

    song_query.prepare(querySql);
    song_query.bindValue(":list_type", QString::number(listType));
    song_query.bindValue(":idx", QString::number(index));
    song_query.bindValue(":id", QString::number(id));
    if (!song_query.exec()) {
        qDebug() << __FILE__ << __LINE__ << "delete error: " << song_query.lastError();
    } else {
        qDebug() << __FILE__ << __LINE__ << "delete success";
    }

    song_query.clear();
}

// 关闭数据库连接
void MusicDB::close()
{
    m_db.close();
    qDebug() << __FILE__ << __LINE__ << "database closed";
}

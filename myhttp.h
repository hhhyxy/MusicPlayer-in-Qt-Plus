#ifndef MYHTTP_H
#define MYHTTP_H

#include "music.h"
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QLabel>

/*
 * 网络请求处理类
 */
class MyHttp : public QObject
{
    Q_OBJECT
public:
    explicit MyHttp(QObject *parent = nullptr);
    ~MyHttp();
    QList<Music> search(QString keywords, int offset = 0, int limit = 80, int type = 1);
    QString searchForSongUrl(int id);
    QStringList searchForSongUrls(QVector<int> musicIdList);
    QMap<int, QString> searchForLrc(int id);

private:
    const QString netease_keywords      = "https://netease.haohao666.top/search";           // 关键词搜索
    const QString netease_songsInfo_Ids = "https://netease.haohao666.top/song/detail?ids="; // 通过id获取歌曲信息
    const QString netease_songUrl_Id    = "https://netease.haohao666.top/song/url?id=";     // 通id获取歌曲播放链接
    const QString netease_songLrc_Id    = "https://netease.haohao666.top/lyric?id=%1";      // 通过id获取歌词

    QString songUrl = "";

    QNetworkAccessManager   *networkManager; // 网络接口管理
    QNetworkRequest         *request;        // 网络请求
    QNetworkReply           *reply;          // 网络应答
    QByteArray              bytes;           // 返回的字节流数据

    QList<Music>        musicList;      // 搜索结果列表
    QVector<int>        musicIdList;    // 歌曲id列表
    QList<QString>      musicUrlList;   // 歌曲链接列表
    QMap<int, QString>  lrcMap;         // 歌词
    QMap<int, QString>  urlMap;         // 歌曲链接

    void searchByUrl(QUrl url);     // 链接搜索
    void parseForMusicId();         // 解析json获取音乐id列表
    void parseForMusicInfo();       // 解析json获取音乐信息列表
    void parseForSongUrl();         // 解析json获取音乐播放链接
    void parseForSongLrc();         // 解析json获取歌词
};

#endif // MYHTTP_H

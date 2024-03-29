﻿#include "myhttp.h"
#include <customitem.h>
#include <QTimer>
#include <QEventLoop>
#include <QJsonParseError>
#include <QDebug>
#include <QUrlQuery>

MyHttp::MyHttp(QObject *parent)
    : QObject{parent}
{
    // 初始化网络请求
    request = new QNetworkRequest();
    networkManager = new QNetworkAccessManager(this);
}

MyHttp::~MyHttp()
{
    delete request;
    request = nullptr;
}

/*
 * @return 搜索结果（音乐列表）
 * @param keywords 搜索关键词
 * @param offset：偏移量
 * @param limit：返回结果数量
 * @param type：搜索类型，取值意义 1: 单曲, 10: 专辑, 100: 歌手, 1000: 歌单,
 *              1002: 用户, 1004: MV, 1006: 歌词, 1009: 电台, 1014: 视频, 1018:综合, 2000:声音(搜索声音返回字段格式会不一样)
*/
QList<Music> MyHttp::search(QString keywords, int offset/* = 0*/, int limit/* = 20*/, int type/* = 1*/)
{
    // 清空歌曲id列表和搜索结果列表
    musicIdList.clear();
    musicList.clear();

    // 拼接关键词搜索链接
    QUrl url(netease_keywords);
    QUrlQuery query;
    query.addQueryItem("keywords", keywords);
    query.addQueryItem("limit", QString::number(limit));
    query.addQueryItem("offset", QString::number(offset));
    query.addQueryItem("type", QString::number(type));
    url.setQuery(query);
    // 搜索关键词并解析返回的Json数据，获取歌曲id列表
    searchByUrl(url);
//    parseForMusicId();

//    // 通过歌曲id列表拼接链接
//    QString urlStr = QString(netease_songsInfo_Ids);
//    for (int i = 0; i < musicIdList.size(); i++) {
//        urlStr.append(QString::number(musicIdList.at(i)));
//        if (i != musicIdList.size() - 1)
//            urlStr.append(",");
//    }
//    // 搜索并解析返回的Json数据，获取歌曲信息列表
//    searchByUrl(QUrl(urlStr));
    parseForMusicInfo();
    // 获取歌曲播放链接
//    searchForSongUrls(musicIdList);
    return musicList;
}

// 获取搜索建议
QStringList MyHttp::searchSuggest(QString keywords)
{
    QString url = netease_searchSuggest.arg(keywords);
    suggest.clear();
    searchByUrl(QUrl(url));
    parseForSuggest();
    return suggest;
}

// 热搜列表
QStringList MyHttp::hotSearch()
{
    hotWords.clear();
    searchByUrl(QUrl(netease_hotSearch));
    parseForHotSearch();
    return hotWords;
}

QList<Music> MyHttp::neteaseHotList()
{
    musicList.clear();
    searchByUrl(QUrl(netease_hotSonglist));
    parseForNeteaseHotList();
    return musicList;
}

/*
 *@return 返回歌曲播放链接
 *@param id:歌曲id
 */
QString MyHttp::searchForSongUrl(int id, QString level)
{
    QUrl url(netease_songUrl_Id);
    QUrlQuery query;
    query.addQueryItem("id", QString::number(id));
    query.addQueryItem("level", level);
    url.setQuery(query);

    searchByUrl(url);
    parseForSongUrl();
    return songUrl;
}

QStringList MyHttp::searchForSongUrls(QVector<int> musicIdList)
{
    QString urlStr = QString(netease_songUrl_Id);
    for (int i = 0; i < musicIdList.size(); i++) {
        urlStr.append(QString::number(musicIdList.at(i)));
        if (i != musicIdList.size() - 1)
            urlStr.append(",");
    }
    searchByUrl(urlStr);
    parseForSongUrl();
    return musicUrlList;
}

/*
 *@return 返回歌词
 *@param id:歌曲id
 */
QMap<int, QString> MyHttp::searchForLrc(int id)
{
    QString url = QString(netease_songLrc_Id).arg(id);
    searchByUrl(url);
    parseForSongLrc();
    return lrcMap;
}

/*
 *@param url:请求链接
 *@func 返回的数据存储到QByteArray中
 */
void MyHttp::searchByUrl(QUrl url)
{
    // 以get方式请求url
    request->setUrl(url);
    reply = networkManager->get(*request);

    //阻塞网络请求，请求成功或5秒后没得到响应，则终止阻塞，
    QEventLoop eventLoop;
    QTimer::singleShot(5000, &eventLoop,
        [&]() {
            if (eventLoop.isRunning()) {
                eventLoop.quit();
            }
        });
    connect(reply, &QNetworkReply::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    // 状态码
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << __FILE__ << __LINE__ << "状态码：" << status_code;

    //response
    if (reply->error() == QNetworkReply::NoError)
    {
        bytes = reply->readAll();
    }
    else
    {
        qDebug()<< __FILE__<<__LINE__<<"searchByUrl_Erro:"<<reply->errorString().toUtf8();
    }
    reply->deleteLater();
}

// 解析QByteArray，得到歌曲id
void MyHttp::parseForMusicId()
{
    // 将json解析为编码为UTF-8的json文档
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);

    // json解析错误处理
    if (json_error.error != QJsonParseError::NoError) {
        qDebug()<< __FILE__<<__LINE__ <<"parseForMusic_Erro:" << json_error.errorString();
        return;
    }

    // 将result下的songs数组提取出来
    QJsonArray songsArray = parse_doucment.object().value("result").toObject().value("songs").toArray();

    int id;  // 歌曲id

    // 获取所有歌曲的id
    for (int i = 0; i < songsArray.size(); i++) {
        // 通过 QJsonArray.at(i)函数获取数组下的第i个元素
        QJsonObject song = songsArray.at(i).toObject();

        // 获取歌曲ID
        id = song.value("id").toInt();
        if (0 == id) {
            qDebug()<< __FILE__<<__LINE__ << "id解析错误";
            return;
        }
        musicIdList.push_back(id);
    }
}

// 解析QByteArray，得到歌曲信息，包括歌名、歌手、专辑名、专辑图片链接、歌曲时长
void MyHttp::parseForMusicInfo()
{
    // 将json解析为编码为UTF-8的json文档
    QJsonParseError json_error;
    QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);

    // json解析错误处理
    if (json_error.error != QJsonParseError::NoError) {
        qDebug()<< __FILE__<<__LINE__ <<"parseForMusicInfo_Erro:" << json_error.errorString();
        return;
    }

    // 将 result 下的 songs 数组提取出来
    QJsonArray songsArray = parse_doucment.object().value("result").toObject().value("songs").toArray();
    if (songsArray.isEmpty()) {
        qDebug()<< __FILE__<<__LINE__ << "songs数组解析错误";
        return;
    }
    int             id;             // 歌曲id
    QString         songName;       // 歌名
    QSet<QString>   authors;        // 歌手
    QString         albumName;      // 专辑名称
    QString         albumPicUrl;    // 专辑图片链接
    int             songDuration;   // 时长

    // 获取所有歌曲的信息
    for (int i = 0; i < songsArray.size(); i++) {
        // 通过 QJsonArray.at(i)函数获取数组下的第i个元素
        QJsonObject song = songsArray.at(i).toObject();

        // 获取歌曲id
        id = song.value("id").toInt(0);
        if(!id) {
            qDebug()<< __FILE__<<__LINE__ << "歌曲id为空";
        }
        // 获取歌曲名称
        songName = song.value("name").toString();
        if (songName.isEmpty()) {
            qDebug()<< __FILE__<<__LINE__ << "歌曲名称为空";
        }

        // 获取歌手名称
        authors.clear();
        QJsonArray authorArr = song.value("ar").toArray();
        for (int i = 0; i < authorArr.size(); i++) {
            authors.insert(authorArr.at(i).toObject().value("name").toString());
        }

        // 获取专辑名称
        albumName = song.value("al").toObject().value("name").toString();
        if (albumName.isEmpty()) {
            albumName = "未知";
            qDebug()<< __FILE__<<__LINE__ << "专辑名称为空";
        }

        // 获取专辑图片链接
        albumPicUrl = song.value("al").toObject().value("picUrl").toString();
        if (albumPicUrl.isEmpty()) {
            qDebug()<< __FILE__<<__LINE__ << "专辑图片链接为空";
        }

        // 获取歌曲时长
        songDuration = song.value("dt").toInt(0);
        if (0 == songDuration) {
            qDebug()<< __FILE__<<__LINE__ << "歌曲时长为空";
        }

        // 存储搜索信息
        Music music(id, songName, QStringList(authors.begin(), authors.end()).join("、"), albumName, albumPicUrl, songDuration);
        musicList.push_back(music);
    }
}

// 解析QByteArray，得到歌曲播放链接
void MyHttp::parseForSongUrl()
{
    // 将json解析未编码未UTF-8的json文档
    QJsonParseError json_error; // 错误信息
    QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
    if (json_error.error != QJsonParseError::NoError) {// 错误处理
        qDebug()<<__FILE__<<__LINE__ <<"JsonUrlErro:" << json_error.errorString();
        return;
    }

    QJsonArray array = parse_doucment.object().value("data").toArray();

    // 将data下的url提取出来

    for (int i = 0; i < array.size(); i++) {
        int id = array[i].toObject().value("id").toInt();
        songUrl = array[i].toObject().value("url").toString();

        if (songUrl.isEmpty()) {
//            qDebug()<< __FILE__<<__LINE__<< id << "songUrl is empty";
            continue;
        }
        urlMap.insert(id, songUrl);
    }
    for (int i = 0; i < musicList.size(); i++) {
        QString url = urlMap.value(musicList.at(i).getId());
        qDebug() << __LINE__ << __FILE__ << songUrl;
        musicUrlList.push_back(url);
//        qDebug()<< __FILE__<<__LINE__<< musicList.at(i).getSongName() << musicList.at(i).getSongUrl();
    }
}

// 解析QByteArray，得到歌词
void MyHttp::parseForSongLrc()
{
    // 将json解析未编码未UTF-8的json文档
    QJsonParseError json_error; // 错误信息
    QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
    if (json_error.error != QJsonParseError::NoError) {// 错误处理
        qDebug()<<__FILE__<<__LINE__ << json_error.errorString();
        return;
    }
    // 获取歌词
    QString lrcs = parse_doucment.object().value("lrc").toObject().value("lyric").toString();
    if (lrcs.isEmpty()) {
        qDebug()<< __FILE__<<__LINE__ << "歌词解析错误";
        return;
    }

    QStringList lrcList = lrcs.split("\n");

    lrcMap.clear();// 清理原来的歌词
    for (int i = 0; i < lrcList.size(); i++) {
        QString str = lrcList.at(i);
        QString lrc = str.trimmed();
        // 歌词
        QString lrcStr = lrc.mid(lrc.lastIndexOf("]") + 1);
        if (lrcStr.isEmpty()) {
            continue;
        }
        //时间解析格式(分*60+秒)*100+厘秒
        int min = lrc.midRef(1, 2).toInt();      //分
        int sec = lrc.midRef(4, 2).toInt();      //秒
        int msec = lrc.midRef(7, 2).toInt();      //毫秒
        int lrcTime = (min * 60 + sec) * 1000 + msec;

        //用Qmap来保存 时间和单行歌词
        lrcMap.insert(lrcTime, lrcStr);
    }

}

// 解析QByteArray，得到搜索建议
void MyHttp::parseForSuggest()
{
    // 将json解析未编码未UTF-8的json文档
    QJsonParseError json_error; // 错误信息
    QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
    if (json_error.error != QJsonParseError::NoError) {// 错误处理
        qDebug() << __FILE__ << __LINE__ << json_error.errorString();
        return;
    }

    QJsonArray array = parse_doucment.object().value("result").toObject().value("allMatch").toArray();

    // 将result/allMatch下的keyword提取出来
    for (int i = 0; i < array.size(); i++) {
//        int id = array[i].toObject().value("id").toInt();
//        songUrl = array[i].toObject().value("url").toString();
        QString word = array[i].toObject().value("keyword").toString();
        suggest.append(word);
    }
}

void MyHttp::parseForHotSearch()
{
    // 将json解析未编码未UTF-8的json文档
    QJsonParseError json_error; // 错误信息
    QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
    if (json_error.error != QJsonParseError::NoError) {// 错误处理
        qDebug() << __FILE__ << __LINE__ << json_error.errorString();
        return;
    }

    QJsonArray array = parse_doucment.object().value("data").toArray();

    // 将data下的searchWord提取出来
    for (int i = 0; i < array.size(); i++) {
        QString word = array[i].toObject().value("searchWord").toString();
        hotWords.append(word);
    }
}

void MyHttp::parseForNeteaseHotList()
{
    // 将json解析未编码未UTF-8的json文档
    QJsonParseError json_error; // 错误信息
    QJsonDocument parse_doucment = QJsonDocument::fromJson(bytes, &json_error);
    if (json_error.error != QJsonParseError::NoError) {// 错误处理
        qDebug() << __FILE__ << __LINE__ << json_error.errorString();
        return;
    }

    QJsonArray array = parse_doucment.object().value("songs").toArray();
    // 将songs下的音乐提取出来
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array.at(i).toObject();
        int id = obj.value("id").toInt();
        QString name = obj.value("name").toString();
        QSet<QString> authors;
        QJsonArray authorArr = obj.value("ar").toArray();
        for (int i = 0; i < authorArr.size(); i++) {
            authors.insert(authorArr.at(i).toObject().value("name").toString());
        }
        QString alName = obj.value("al").toObject().value("name").toString();
        QString picUrl = obj.value("al").toObject().value("picUrl").toString();
        int dt = obj.value("dt").toInt();
        musicList.push_back(Music(id, name, QStringList(authors.begin(), authors.end()).join("、"), alName, picUrl, dt));
    }
}



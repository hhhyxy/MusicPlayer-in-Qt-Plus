#ifndef MUSIC_H
#define MUSIC_H

#include <QPixmap>

/*
 * 音乐信息存储类
 */
class Music
{
public:
    explicit Music();
    explicit Music(int id, QString songName, QString author, QString albumName, QString albumPicUrl, int songDuration);
    // 重写==操作符
    bool operator ==(const Music &music);
    // 判空
    bool empty();
    // geter
    int getId() const;
    QString getSongName() const;
    QString getAuthor() const;
    QString getAlbumName() const;
    QString albumPicUrl() const;
    QString getSongDuration() const;
    int songDuration() const;

private:
    int         m_id = 0;          // 歌曲Id
    QString     m_songName;        // 歌曲名称
    QString     m_author;          // 歌手名称
    QString     m_albumName;       // 专辑名称
    QString     m_albumPicUrl;     // 专辑图片链接
    QString     m_songUrl;         // 歌曲播放链接
    int         m_songDuration;    // 歌曲时长
};

// 注册Qariant
Q_DECLARE_METATYPE(Music)

#endif // MUSIC_H

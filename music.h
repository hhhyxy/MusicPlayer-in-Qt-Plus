#ifndef MUSIC_H
#define MUSIC_H
#include <QString>
#include <QPixmap>

class Music
{
public:
    Music();
    Music(int id, QString songName, QString author, QString albumName, QString albumPicUrl, int songDuration);
    Music(int id, QString songName, QString author, QString albumName, QString albumPicUrl, int songDuration, QString songUrl);

    // geter and setter
    int getId() const;

    QString getSongName() const;

    QString getAuthor() const;

    QString getAlbumName() const;

    QString getSongUrl() const;

    QString albumPicUrl() const;

    QString getSongDuration() const;

    int songDuration() const;

    void setSongUrl(const QString &newSongUrl);

    bool operator ==(const Music &music);

    void setSongName(const QString &newSongName);

    void setAuthor(const QString &newAuthor);

    void setAlbumName(const QString &newAlbumName);

    void setAlbumPicUrl(const QString &newAlbumPicUrl);

    void setSongDuration(int newSongDuration);

    QPixmap albumPic() const;
    void setAlbumPic(const QPixmap &newAlbumPic);

private:
    int         m_id = 0;               // 歌曲Id
    QString     m_songName = "";        // 歌曲名称
    QString     m_author = "";          // 歌手
    QString     m_albumName = "";       // 专辑名称
    QString     m_albumPicUrl = "";     // 专辑图片链接
    QPixmap     m_albumPic;             // 专辑图片
    int         m_songDuration = 0;     // 歌曲时长
    QString     m_songUrl = "";         // 歌曲链接

};

#endif // MUSIC_H

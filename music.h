#ifndef MUSIC_H
#define MUSIC_H
#include <QString>
#include <QPixmap>



class Music
{
public:
    explicit Music();
    explicit Music(int id, QString songName, QString author, QString albumName, QString albumPicUrl, int songDuration);
    explicit Music(int id, QString songName, QString author, QString albumName, QString albumPicUrl, int songDuration, QString songUrl);

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

private:
    int         m_id;              // 歌曲Id
    QString     m_songName;        // 歌曲名称
    QString     m_author;          // 歌手
    QString     m_albumName;       // 专辑名称
    QString     m_albumPicUrl;     // 专辑图片链接
    int         m_songDuration;    // 歌曲时长
    QString     m_songUrl;         // 歌曲链接

};
Q_DECLARE_METATYPE(Music);
#endif // MUSIC_H

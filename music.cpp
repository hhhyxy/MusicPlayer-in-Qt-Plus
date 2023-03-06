#include "music.h"
#include <QTime>

Music::Music()
{

}

Music::Music(int id, QString songName, QString author, QString albumName, QString albumPicUrl, int songDuration, QString songUrl)
    :m_id(id),
    m_songName(songName),
    m_author(author),
    m_albumName(albumName),
    m_albumPicUrl(albumPicUrl),
    m_songDuration(songDuration),
    m_songUrl(songUrl)
{
}

bool Music::operator ==(const Music &music)
{
    return this->getId() == music.getId();
}

void Music::setSongName(const QString &newSongName)
{
    m_songName = newSongName;
}

void Music::setAuthor(const QString &newAuthor)
{
    m_author = newAuthor;
}

void Music::setAlbumName(const QString &newAlbumName)
{
    m_albumName = newAlbumName;
}

QString Music::albumPicUrl() const
{
    return m_albumPicUrl;
}

void Music::setAlbumPicUrl(const QString &newAlbumPicUrl)
{
    m_albumPicUrl = newAlbumPicUrl;
}

void Music::setSongDuration(int newSongDuration)
{
    m_songDuration = newSongDuration;
}

QString Music::getSongDuration() const
{
    return QTime(0, 0, 0, 0).addMSecs(m_songDuration).toString("mm:ss");
}

int Music::songDuration() const
{
    return m_songDuration;
}

void Music::setSongUrl(const QString &newSongUrl)
{
    m_songUrl = newSongUrl;
}

QString Music::getSongUrl() const
{
    return m_songUrl;
}

QString Music::getAuthor() const
{
    if (m_author.isEmpty())
        return "未知";
    return m_author;
}

QString Music::getAlbumName() const
{
    if (m_albumName.isEmpty())
        return "未知";
    return m_albumName;
}

QString Music::getSongName() const
{
    if (m_songName.isEmpty())
        return "未知";
    return m_songName;
}


int Music::getId() const
{
    return m_id;
}

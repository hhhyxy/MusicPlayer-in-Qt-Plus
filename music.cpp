#include "music.h"
#include <QTime>

Music::Music()
{

}

Music::Music(int id, QString songName, QString author, QString albumName, QString albumPicUrl, int songDuration)
    :m_id(id),
    m_songName(songName),
    m_author(author),
    m_albumName(albumName),
    m_albumPicUrl(albumPicUrl),
    m_songDuration(songDuration)
{
}

bool Music::operator ==(const Music &music)
{
    return this->getId() == music.getId();
}

bool Music::empty()
{
    if (m_id)
        return false;
    return true;
}

QString Music::albumPicUrl() const
{
    return m_albumPicUrl;
}

QString Music::getSongDuration() const
{
    return QTime(0, 0, 0, 0).addMSecs(m_songDuration).toString("mm:ss");
}

int Music::songDuration() const
{
    return m_songDuration;
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

Music::Music(int id, const QString &songName, const QString &author, const QString &albumPicUrl) : m_id(id),
    m_songName(songName),
    m_author(author),
    m_albumPicUrl(albumPicUrl)
{}

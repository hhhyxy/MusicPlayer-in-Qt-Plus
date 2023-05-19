#include "musiclist.h"

MusicList::MusicList()
{

}

MusicList::MusicList(int id, const QString &name, const QString &cover, const QString &intro) : m_id(id),
    m_name(name),
    m_cover(cover),
    m_intro(intro)
{}

int MusicList::id() const
{
    return m_id;
}

QString MusicList::name() const
{
    return m_name;
}

void MusicList::setName(const QString &newName)
{
    m_name = newName;
}

QString MusicList::cover() const
{
    return m_cover;
}

void MusicList::setCover(const QString &newCover)
{
    m_cover = newCover;
}

QString MusicList::intro() const
{
    return m_intro;
}

void MusicList::setIntro(const QString &newIntro)
{
    m_intro = newIntro;
}

MusicList::MusicList(const QString &name, const QString &cover, const QString &intro) : m_name(name),
    m_cover(cover),
    m_intro(intro)
{}

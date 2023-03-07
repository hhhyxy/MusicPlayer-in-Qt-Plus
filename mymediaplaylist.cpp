#include "mymediaplaylist.h"
#include <QList>
MyMediaPlaylist::MyMediaPlaylist(QObject *parent)
    : QMediaPlaylist {parent}
{
    search = new MyHttp(this);

}

int MyMediaPlaylist::mediaCount() const
{
    return m_idList.size();
}

QMediaContent MyMediaPlaylist::media(int index) const
{
    qDebug()<<"media";
    if (index < 0 || index >= m_idList.size())
        return QMediaContent();
    int id = m_idList.at(index);
    QString url = search->searchForSongUrl(id);
    qDebug() << __FILE__ << __LINE__ << url;
    return QMediaContent(QUrl(url));
}

QMediaContent MyMediaPlaylist::currentMedia() const
{
    return media(m_currentIndex);
}

int MyMediaPlaylist::currentIndex() const
{
    return m_currentIndex;
}

void MyMediaPlaylist::setCurrentIndex(int index, bool songSwitch)
{
    qDebug() << __FILE__ << __LINE__ << "setCurrentIndex";
    if (index < 0 || index >= m_idList.size())
        return;
    m_currentIndex = index;
    m_recentIndex.push_back(index);
    if (!songSwitch)
        return;
    int id = m_idList.at(index);
    QString url = search->searchForSongUrl(id);
    if (url.isEmpty()) {
        m_currentIndex++;
        setCurrentIndex(m_currentIndex);
        qDebug() << __FILE__ << __LINE__ << "song url is null";
        return;
    }
    Q_EMIT  currentIndexChanged(m_currentIndex);
    Q_EMIT  currentMediaChanged(QMediaContent(url));
}

int MyMediaPlaylist::nextIndex(int steps)
{
    qDebug() << __FILE__ << __LINE__ << "nextIndex";
    int next;
    switch (playbackMode()) {
    case QMediaPlaylist::Random:
        if (m_shuffleList.isEmpty()) {
            shuffle();
            qDebug() << __FILE__ << __LINE__ << "shuffle";
        }else {
            int index = m_shuffleList.takeFirst();
            return index;
        }
        break;
    case QMediaPlaylist::Sequential:
        next = m_currentIndex + steps;
        if (next >= m_idList.size())
            return -1;
        break;
    case QMediaPlaylist::Loop:
        next = m_currentIndex + steps;
        if (next >= m_idList.size())
            return 0;
        break;
    case QMediaPlaylist::CurrentItemInLoop:
        return m_currentIndex;
        break;
    default:
        break;
    }
    return next;
}

int MyMediaPlaylist::previousIndex(int steps)
{
    if (!m_recentIndex.isEmpty()) {
        m_recentIndex.pop_back();
        int prev = m_recentIndex.last();
        m_recentIndex.pop_back();
        qDebug() << __FILE__ << __LINE__ << "previousIndex";
        return prev;
    }
    return -1;
}

void MyMediaPlaylist::shuffle()
{
    // 生成随机播放列表
    m_shuffleList.clear();
    int count = m_idList.size();
    for (int i = 0; i < count; i++) {
        m_shuffleList.append(i);
    }
    std::random_shuffle(m_shuffleList.begin(), m_shuffleList.end());
}

bool MyMediaPlaylist::addMedia(int id)
{
    qDebug() << __FILE__ << __LINE__ << "addMedia";
    m_idList.push_back(id);
    Q_EMIT  mediaInserted(m_idList.size() - 1, m_idList.size() - 1);
    return true;
}

bool MyMediaPlaylist::insertMedia(int index, const int id)
{
    qDebug() << __FILE__ << __LINE__ << "insertMedia";
    if (index < 0 || index > m_idList.size())
        return false;
    m_idList.insert(index, id);
    Q_EMIT  mediaInserted(index, index);
    return true;
}

bool MyMediaPlaylist::removeMedia(int index)
{
    if (index < 0 || index >= m_idList.size())
        return false;
    m_idList.removeAt(index);
    Q_EMIT  mediaRemoved(index, index);
    return true;
}

bool MyMediaPlaylist::clear()
{
    qDebug() << __FILE__ << __LINE__ << "clear";
    m_idList.clear();
}

void MyMediaPlaylist::next()
{
    qDebug() << __FILE__ << __LINE__ << "next";
    setCurrentIndex(nextIndex());
}

#include "mymediaplaylist.h"
#include <QList>
#include <QDebug>
MyMediaPlaylist::MyMediaPlaylist(QObject *parent)
    : QMediaPlaylist {parent}
{
    search = new MyHttp(this);

}

// 播放列表音乐数量
int MyMediaPlaylist::mediaCount() const
{
    return m_idList.size();
}

// 通过播放列表索引获取音乐
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

// 当前索引对应音乐
QMediaContent MyMediaPlaylist::currentMedia() const
{
    return media(m_currentIndex);
}

// 当前音乐在播放列表中的序号
int MyMediaPlaylist::currentIndex() const
{
    return m_currentIndex;
}

// 设置当前播放的音乐
void MyMediaPlaylist::setCurrentIndex(int index)
{
    qDebug() << __FILE__ << __LINE__ << "setCurrentIndex";
    // 判断索引是否合法
    if (index < 0 || index >= m_idList.size())
        return;
    // 更新当前索引
    m_currentIndex = index;
    // 加入历史播放列表
    m_recentIndex.push_back(index);
    // 通过索引获取当前音乐id，在此基础上获取音乐播放链接
    int id = m_idList.at(index);
    QString url = search->searchForSongUrl(id);
    // 若播放链接为空，播放下一首
    if (url.isEmpty()) {
        qDebug() << __FILE__ << __LINE__ << "song url is null";
        m_currentIndex++;
        setCurrentIndex(m_currentIndex);
        return;
    }
    Q_EMIT  currentIndexChanged(m_currentIndex);
    Q_EMIT  currentMediaChanged(QMediaContent(url));
}

// 获取steps首后的音乐索引
int MyMediaPlaylist::nextIndex(int steps)
{
    qDebug() << __FILE__ << __LINE__ << "nextIndex";
    int next = m_currentIndex;
    // 根据播放模式，确定下一首播放的音乐
    switch (playbackMode()) {
    case QMediaPlaylist::Random:            // 随机播放
        // 如果随机播放列表为空，则获取随机播放列表
        if (m_shuffleList.isEmpty()) {
            shuffle();
            qDebug() << __FILE__ << __LINE__ << "shuffle";
        }
        next = m_shuffleList.takeFirst();
        break;
    case QMediaPlaylist::Sequential:        // 顺序播放
        next = m_currentIndex + steps;
        if (next >= m_idList.size())
            return -1;
        break;
    case QMediaPlaylist::Loop:              // 列表循环
        next = m_currentIndex + steps;
        if (next >= m_idList.size())
            return 0;
        break;
    case QMediaPlaylist::CurrentItemInLoop: // 单曲循环
        return m_currentIndex;
        break;
    default:
        break;
    }
    return next;
}

// 获取前steps首的音乐索引
int MyMediaPlaylist::previousIndex(int steps)
{
    if (!m_recentIndex.isEmpty()) {
        while (steps-- > 0) {
            m_recentIndex.pop_back();
        }
        qDebug() << __FILE__ << __LINE__ << "previousIndex";
        return m_recentIndex.takeLast();
    }
    return -1;
}

// 雪花算法，生成随机播放列表
void MyMediaPlaylist::shuffle()
{
    // 清空随机列表
    m_shuffleList.clear();
    // 生成与id列表等长的递增数列，0，1，2，3....
    int count = m_idList.size();
    for (int i = 0; i < count; i++) {
        m_shuffleList.append(i);
    }
    // 打乱递增数列的顺序
    std::random_shuffle(m_shuffleList.begin(), m_shuffleList.end());
    m_shuffleList.swapItemsAt(0, m_shuffleList.size()>>1);
}

// 在最后添加音乐
bool MyMediaPlaylist::addMedia(int id)
{
    m_idList.push_back(id);
    Q_EMIT  mediaInserted(m_idList.size() - 1, m_idList.size() - 1);
    return true;
}

// 指定位置插入音乐
bool MyMediaPlaylist::insertMedia(int index, const int id)
{
    qDebug() << __FILE__ << __LINE__ << "insertMedia";
    // 索引合法性检验
    if (index < 0 || index > m_idList.size())
        return false;
    m_idList.insert(index, id);
    Q_EMIT  mediaInserted(index, index);
    return true;
}

// 删除音乐
bool MyMediaPlaylist::removeMedia(int index)
{
    // 索引合法性检验
    if (index < 0 || index >= m_idList.size())
        return false;
    // 移除指定音乐
    m_idList.removeAt(index);
    shuffle();
    Q_EMIT  mediaRemoved(index, index);
    return true;
}

// 清空播放列表
bool MyMediaPlaylist::clear()
{
    qDebug() << __FILE__ << __LINE__ << "clear";
    if (!m_idList.isEmpty())
        m_idList.clear();       // 清空id列表
    if (!m_shuffleList.isEmpty())
        m_shuffleList.clear();  // 清空随机播放列表
    return true;
}

// 播放上一首
void MyMediaPlaylist::last()
{
    setCurrentIndex(previousIndex());
}

// 播放下一首
void MyMediaPlaylist::next()
{
    qDebug() << __FILE__ << __LINE__ << "next";
    setCurrentIndex(nextIndex());
}

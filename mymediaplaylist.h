#ifndef MYMEDIAPLAYLIST_H
#define MYMEDIAPLAYLIST_H

#include <QMediaPlaylist>
#include "myhttp.h"

class MyMediaPlaylist : public QMediaPlaylist
{
    Q_OBJECT
public:
    explicit MyMediaPlaylist(QObject *parent = nullptr);
    int mediaCount() const;
    QMediaContent media(int index) const;
    QMediaContent currentMedia() const;
    int currentIndex() const;
    void setCurrentIndex(int index/*, bool songSwitch = true*/);
    int nextIndex(int steps = 1);
    int previousIndex(int steps = 1);
    void shuffle();
    bool addMedia(int id);
    bool insertMedia(int index, const int id);
    bool removeMedia(int index);
    bool clear();
    void last();
    void next();

private:
    QList<int> m_idList;
    int m_currentIndex = -1;
    MyHttp *search;
    QList<int> m_shuffleList;
    QList<int> m_recentIndex;
};

#endif // MYMEDIAPLAYLIST_H

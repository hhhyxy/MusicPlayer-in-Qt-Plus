#ifndef MUSICLIST_H
#define MUSICLIST_H

#include <QObject>
class MusicList
{
public:
    MusicList();
    MusicList(const QString &name, const QString &cover = "https://p2.music.126.net/6y-UleORITEDbvrOLV0Q8A==/5639395138885805.jpg", const QString &intro = "");
    MusicList(int id, const QString &name, const QString &cover = "https://p2.music.126.net/6y-UleORITEDbvrOLV0Q8A==/5639395138885805.jpg", const QString &intro = "");

    int id() const;
    QString name() const;
    void setName(const QString &newName);
    QString cover() const;
    void setCover(const QString &newCover);
    QString intro() const;
    void setIntro(const QString &newIntro);

private:
    int m_id;
    QString m_name;
    QString m_cover;
    QString m_intro;
};

// 注册Qariant
Q_DECLARE_METATYPE(MusicList)

#endif // MUSICLIST_H

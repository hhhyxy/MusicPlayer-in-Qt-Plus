#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include "music.h"
#include "customitem.h"
#include <QListWidget>
/*
 * 搜索列表、我喜欢的音乐列表、歌单列表等用于音乐显示的列表
 */
class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent = nullptr);
    // 设置音乐列表
    void setMusicList(const QList<Music> &musicList);
    // 设置列表类型
    void setListType(int listType);
signals:
    // item双击信号
    void customItemDoubleClicked(CustomItem *item);
    // 菜单点击信号
    void menuClicked(CustomItem *item, int itemType);
private:
    // 添加item
    void addCustomItem(const Music music);
    // 添加items
    void addCustomItems(int begin, int end);
    void loadMore();
private slots:
    void onScrollBarValueChange(int value);
private:
    QList<Music> m_musicList;
    int listType;
    int itemsNum;
};

#endif // MYLISTWIDGET_H

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
    // 列表类型
    enum ListType {
        SEARCHRESULT,
        LOCAL,
        HISTORY,
        FAVORITE,
        DEFALUT
    };
    Q_ENUM(ListType)
    void init (int id, QString name);
    // 设置音乐列表
    void setMusicList(const QList<Music> &musicList);
    void insertCustomItem(Music music, int row = 0);
    void removeCustomItem(CustomItem *item);
    // 获取列表类型
    int getListType();

signals:
    // item双击信号
    void customItemDoubleClicked(CustomItem *item);
    // 菜单点击信号
    void menuClicked(CustomItem *item, int itemType);
    void loaded(int id);
    void cleared(int id);
private:
    // 添加item
    void addCustomItem(const Music music, int row = -1);
    // 添加items
    void addCustomItems(int begin, int end);
    // 加载更多
    void loadMore();
    // 清空列表
    void clearList();
protected:
    void mouseReleaseEvent(QMouseEvent *event);
private slots:
    void onScrollBarValueChange(int value);
private:
    int id;
    QString name;
    bool loadmore = true;
    int itemsNum;
    QList<Music> m_musicList;
    QList<CustomItem *> items;
};

#endif // MYLISTWIDGET_H

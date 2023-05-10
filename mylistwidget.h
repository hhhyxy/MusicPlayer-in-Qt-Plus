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
    void push_back(Music music);
    void push_front(Music music);
    void pop_front();
    void pop_back();
    void takeAt(int index);
    // 设置音乐列表
    void setMusicList(const QList<Music> &musicList);
    void insertCustomItem(Music music, int row = 0);
    void removeCustomItem(CustomItem *item);
    void removeCustomItem(int index);
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
    void onCustomItemDoubleClicked(CustomItem *item);
    void onSliderPressed();
    void onSliderReleased();
private:
    int id; // 列表id
    QString name;   // 歌单名

    int verticalBarValue = 0;   // 垂直滚动条数值
    bool scrollBarOn = false;   // 垂直滚动条是否可见
    bool sliderPressed = false; // 垂直滚动条是否按下

    int itemsNum;           // 歌曲数
    bool loadmore = true;   // 是否可以加载更多

    QList<Music> m_musicList;   // 音乐列表
    QList<CustomItem *> items;  // 自定义的items
    CustomItem *doubleClickedItem = nullptr;    // 被双击的item
};

#endif // MYLISTWIDGET_H

#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include "music.h"
#include <QWidget>
#include <QMenu>

namespace Ui {
class CustomItem;
}
/*
 * 搜索结果列表、歌单列表等列表的某一行
 */
class CustomItem : public QWidget
{
    Q_OBJECT
public:
    // item类型（枚举类型）
    enum {
        SEARCHRESULT,
        FAVORITE,
        SONGLIST,
        LOCAL,
        RECENTLY,
        PLAY,
        ADDTOFAVORITE,
        REMOVEFROMFAVORITE,
        ADDTOSONGLIST,
        REMOVEFROMSONGLIST
    };
    explicit CustomItem(Music music, QWidget *parent = nullptr);
    ~CustomItem();
    // 获取item对应的音乐
    Music getMusic() const;
    // 改变字体颜色
    void changeFontColor(QString color);
    // 获取专辑图片
    QPixmap getAlbumPic();
    // 设置item类型
    void setItemType(int itemType);
    // 获取item类型
    int getItemType() const;
signals:
    // 菜单点击信号
    void menuClicked(CustomItem *item, int type);
    // item双击事件
    void myItemDoubleClicked(CustomItem *item);
protected:
    // 双击事件
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    Ui::CustomItem *ui;

    QMenu   *menu = nullptr;    // 菜单
    Music   music;              // item对应的音乐
    int     itemType;           // item类型
    // 初始化菜单
    void initMenu();
    // 显示右键菜单
    void showMenu();
};

#endif // CUSTOMITEM_H

#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include "music.h"
#include <QWidget>
#include <QMenu>

namespace Ui {
class CustomItem;
}

class CustomItem : public QWidget
{
    Q_OBJECT


public:
    enum {
        SEARCHRESULT,
        FAVORITE,
        SONGLIST,
        LOCAL,
        RECENTLY
    };
    explicit CustomItem(Music music, QWidget *parent = nullptr);

    ~CustomItem();

    Music getMusic() const;

    // 改变字体颜色
    void changeFontColor(QString color);
    // 获取专辑图片
    QPixmap getAlbumPic();
    // 设置item类型
    void setItemType(int itemType);
    int getItemType() const;

signals:
    // 播放
    void musicPlay(CustomItem *item);
    // 添加到我喜欢的音乐
    void addToMyFavoriteMusic(CustomItem *item);
    // 添加到歌单
    void addToSonglist(CustomItem *item);
    // 双击信号
    void myItemDoubleClicked(CustomItem *item);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    Ui::CustomItem *ui;

    QMenu   *menu = nullptr;
    Music   music;
    int     itemType;
    // 显示专辑图片
    void showAlbumPic();
    // 初始化菜单
    void initMenu();
    // 显示右键菜单
    void showMenu();
};

#endif // CUSTOMITEM_H

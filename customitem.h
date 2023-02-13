#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include "music.h"
#include <QWidget>
#include <QMenu>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class CustomItem;
}

class CustomItem : public QWidget
{
    Q_OBJECT

public:
    explicit CustomItem(Music music, QWidget *parent = nullptr);

    ~CustomItem();

    Music getMusic() const;

    // 改变字体颜色
    void changeFontColor(QString color);
    // 获取专辑图片
    QPixmap getAlbumPic();
    // 显示信息
    void showInfo();
signals:
    // 播放
    void musicPlay(CustomItem * item);
    // 添加到我喜欢的音乐
    void addToMyFavoriteMusic(CustomItem * item);
    // 添加到歌单
    void addToSonglist(CustomItem * item);

protected:
    void paintEvent(QPaintEvent *event);


private:
    Ui::CustomItem          *ui;
    QMenu *menu = nullptr;
    Music music;

    bool ischange = false;
    // 显示专辑图片
    void showAlbumPic();
    // 初始化菜单
    void initMenu();
    // 显示右键菜单
    void showMenu();
};

#endif // CUSTOMITEM_H

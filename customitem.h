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

    static QPixmap image2Radius(QPixmap pixmap, int radius = 0);
    // 改变字体颜色
    void changeFontColor(QString color);
protected:
    void paintEvent(QPaintEvent *event);
signals:
    // 播放
    void musicPlay(CustomItem * item);
    // 添加到我喜欢的音乐
    void addToMyFavoriteMusic(CustomItem * item);
    // 添加到歌单
    void addToSonglist(CustomItem * item);
private:
    Ui::CustomItem          *ui;
    QNetworkAccessManager   *networkManager;
    QNetworkRequest         *request;
    QMenu *menu = nullptr;
    Music music;
//    bool albumPicLoadingFinished = false;  // 图片是否加载完成
bool ischange = false;
    // 显示专辑图片
    void showAlbumPic();
    // 初始化菜单
    void initMenu();
    // 显示右键菜单
    void showMenu();

private slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // CUSTOMITEM_H

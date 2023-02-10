#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include "music.h"
#include <QWidget>
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

    void setData();

    Music getMusic() const;

    static QPixmap image2Radius(QPixmap pixmap, int radius = 0);

    bool albumPicLoadingIsFinished() const;

    void changeFontColor(QString color);
private:
    Ui::CustomItem          *ui;
    QNetworkAccessManager   *networkManager;
    QNetworkRequest         *request;

    Music music;
    bool albumPicLoadingFinished = false;  // 图片是否加载完成
bool ischange = false;
    void showAlbumPic();

private slots:
    void replyFinished(QNetworkReply *reply);
};

#endif // CUSTOMITEM_H

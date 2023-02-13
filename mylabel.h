#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>


class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);
    ~MyLabel();
    void setRadiusPixmap(QString url);
    void makeRadiusPixmap(QPixmap pixmap);
    QPixmap getImg() const;

signals:
//    void signalSetPixmap(QPixmap pixmap);
private:
    QNetworkAccessManager   *networkManager;
    QNetworkRequest         *request;
    QPixmap img;

    void onPicReplyFinished(QNetworkReply *reply);
};

#endif // MYLABEL_H

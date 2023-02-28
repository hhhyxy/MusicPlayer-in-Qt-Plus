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
    void makeRadiusPixmap(QByteArray bytes);
    QPixmap getImg() const;

private:
    QNetworkAccessManager   *networkManager = nullptr;
    QNetworkRequest         *request = nullptr;
    QPixmap img;
    QString picUrl = "";



private slots:
    void onReplyFinished(QNetworkReply *reply);
};

#endif // MYLABEL_H

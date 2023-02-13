#include "mylabel.h"
#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QNetworkReply>
#include <QtConcurrent>

MyLabel::MyLabel(QWidget *parent)
    : QLabel{parent}
{
    // 初始化网络请求
    networkManager = new QNetworkAccessManager(this);
    request = new QNetworkRequest();
}

MyLabel::~MyLabel()
{
    delete request;
}

void MyLabel::makeRadiusPixmap(QPixmap pixmap)
{
    //处理大尺寸的图片
    if (pixmap.width() > 600) {
        pixmap = pixmap.scaledToWidth(600);
    }
    QSize size = pixmap.size();
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    int radius = size.width() * 0.07;
    painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius);//修改这个值，可以改弧度，和直径相等就是圆形
    pixmap.setMask(mask);
    img = pixmap;
//    this->setPixmap(pixmap);
    QMetaObject::invokeMethod(this, "setPixmap", Q_ARG(QPixmap, pixmap));
}

QPixmap MyLabel::getImg() const
{
    return img;
}

void MyLabel::setRadiusPixmap(QString url)
{
    connect(networkManager, QNetworkAccessManager::finished, this, MyLabel::onPicReplyFinished);
    request->setUrl(QUrl(url));
    networkManager->get(*request);
}

void MyLabel::onPicReplyFinished(QNetworkReply *reply)
{
    // 获取响应状态码，200表示正常
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<__FILE__<<__LINE__ << tr("状态码：") << status_code;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();  //获取字节
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        QtConcurrent::run(this, &MyLabel::makeRadiusPixmap, pixmap);
//        this->makeRadiusPixmap(pixmap);
    }else {
        qDebug()<<__FILE__<<__LINE__<<"获取专辑图片失败";
    }
    reply->deleteLater();
    reply = nullptr;
}

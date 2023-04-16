#include "mylabel.h"

#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QNetworkReply>
#include <QtConcurrent>


MyLabel::MyLabel(QWidget *parent)
    : QLabel{parent}
{
    // 初始化网络请求
    networkManager = new QNetworkAccessManager(this);
    request = new QNetworkRequest();
    setShadow();
}

MyLabel::~MyLabel()
{
    delete request;
}

// 获取专辑图片
QPixmap MyLabel::getImg() const
{
    if (imgLoaded)
        return img;
    return QPixmap();
}

// 设置阴影
void MyLabel::setShadow()
{
    //实例阴影shadow
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    //设置阴影距离
    shadow->setOffset(0, 0);
    //设置阴影颜色
    shadow->setColor(QColor("#444444"));
    //设置阴影长度
    shadow->setBlurRadius(5);
    //给窗体设置阴影
    this->setGraphicsEffect(shadow);
}

void MyLabel::setRadiusPixmap(QString url)
{
    if (picUrl == url)
        return;
    picUrl = url;
    request->setUrl(QUrl(url));
    networkManager->get(*request);
    connect(networkManager, QNetworkAccessManager::finished, this, MyLabel::onReplyFinished, Qt::UniqueConnection);
}

//void MyLabel::setRoundedPicInThread(QString url)
//{
//    downLoader->downLoad(url);
//    connect(downLoader, &ThreadDownLoader::finished, this, [=](QByteArray bytes) {
//        QtConcurrent::run(this, &MyLabel::makeRadiusPixmap, bytes);
//    });
//}

// 图片下载完成
void MyLabel::onReplyFinished(QNetworkReply *reply)
{
    // 获取响应状态码，200表示正常
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<__FILE__<<__LINE__ << "状态码：" << status_code;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();  //获取字节
        QtConcurrent::run(this, QOverload<QByteArray>::of(&MyLabel::makeRadiusPixmap), bytes);
    }else {
        qDebug()<<__FILE__<<__LINE__<<"获取专辑图片失败";
    }
    reply->deleteLater();
}

// 把图片处理为圆角并显示
void MyLabel::makeRadiusPixmap(QByteArray bytes)
{
    QPixmap pixmap;
    pixmap.loadFromData(bytes);
    if (pixmap.isNull())
        return;
    //处理大尺寸的图片
    pixmap = pixmap.scaledToWidth(this->width());
//    pixmap = pixmap.scaledToWidth(600);
    makeRadiusPixmap(pixmap);

}

void MyLabel::makeRadiusPixmap(QPixmap pixmap)
{
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
    imgLoaded = true;
    QMetaObject::invokeMethod(this, "setPixmap", Q_ARG(QPixmap, pixmap));
}



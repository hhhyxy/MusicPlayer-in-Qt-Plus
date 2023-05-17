#include "mylabel.h"

#include <QPainter>
#include <QBitmap>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>
#include <QNetworkReply>
#include <QtConcurrent>
#include "gaussianblur.h"

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
        return this->pixmap(Qt::ReturnByValue);
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

void MyLabel::setBackRole(int show)
{
    backRole = show;
}

void MyLabel::setRadiusPixmap(QString url)
{
    if (picUrl == url)
        return;
    picUrl = url;
    int width = this->width();
    url += QString("?param=%1y%2").arg(width).arg(width);
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
    qDebug() << __FILE__ << __LINE__ << "状态码：" << status_code;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();  //获取字节
        if (!backRole)
            QtConcurrent::run(this, QOverload<QByteArray>::of(&MyLabel::makeRadiusPixmap), bytes);
        else
            QtConcurrent::run(this, &MyLabel::makeRadiusBlurPixmap, bytes);
    }else {
        qDebug() << __FILE__ << __LINE__ << "获取专辑图片失败";
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
    makeRadiusPixmap(pixmap);
}

void MyLabel::makeRadiusBlurPixmap(QByteArray bytes)
{
    QImage img;
    img.loadFromData(bytes);
    GaussianBlur blur;
    blur.gaussBlur(img, 300);
    makeRadiusPixmap(QPixmap::fromImage(img).scaled(this->size()));
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
    radius = size.width() * 0.1;
    if (backRole)
        radius = 6;
    painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius);//修改这个值，可以改弧度，和直径相等就是圆形
    pixmap.setMask(mask);

    imgLoaded = true;
    // 显示图片
    std::lock_guard<std::mutex> lck(mutex);
    QMetaObject::invokeMethod(this, "setPixmap", Q_ARG(QPixmap, pixmap.scaled(this->size())));
}



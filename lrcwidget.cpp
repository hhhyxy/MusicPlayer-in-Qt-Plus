#include "lrcwidget.h"

#include <QPaintEvent>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPalette>
#include <QImage>
#include <QtConcurrent>
#include <QLabel>

LrcWidget::LrcWidget(QWidget *parent)
    : QWidget{parent}
{
    manager = new QNetworkAccessManager(this);
    gauss = new GaussianBlur(this);
    connect(manager, QNetworkAccessManager::finished, this, LrcWidget::onRequestFinished);
    connect(gauss, GaussianBlur::finished, this, LrcWidget::onGaussianFinished);
}

void LrcWidget::setGaussblurBackground(const QString &url)
{
//    draw = true;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    manager->get(request);
}

void LrcWidget::repaintBackground()
{
    if (img.isNull())
        return;
    // 绘制背景图片
    this->setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(img.scaled(this->size())));
    this->setPalette(pal);
    qDebug()<<"update";
}

void LrcWidget::paintEvent(QPaintEvent *event)
{
    repaintBackground();
    QWidget::paintEvent(event);
}

void LrcWidget::onRequestFinished(QNetworkReply *reply)
{
    // 获取响应状态码，200表示正常
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<__FILE__<<__LINE__ << "状态码：" << status_code;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();  //获取字节
        QImage img;
        img.loadFromData(bytes);
//        gauss.gaussBlur(img, 200);
        QtConcurrent::run(gauss, GaussianBlur::gaussBlur, img, 300);

//        QLabel *label = new QLabel(this);
//        QPixmap pixmap;
//        pixmap.convertFromImage(img);
//        label->setPixmap(pixmap);
//        label->setGeometry(this->rect());
//        label->show();

        // 绘制背景图片
//        QPalette pal = this->palette();
//        pal.setBrush(QPalette::Window, QBrush(img));
//        this->setPalette(pal);
    }else {
        qDebug()<<__FILE__<<__LINE__<<"获取专辑图片失败";
    }
    reply->deleteLater();
}

void LrcWidget::onGaussianFinished(QImage image)
{
    img = image;
    repaintBackground();
}

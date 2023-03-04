#include "lrcwidget.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPalette>
#include <QImage>
#include <QPixmap>
#include <QBitmap>
#include <QtConcurrent>
#include <QPainterPath>

LrcWidget::LrcWidget(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    manager = new QNetworkAccessManager(this);
    gauss = new GaussianBlur(this);
    radius = 500;

    // 网络请求完成
    connect(manager, QNetworkAccessManager::finished, this, LrcWidget::onRequestFinished);
    // 图片模糊处理完成
    connect(gauss, GaussianBlur::finished, this, LrcWidget::onGaussianFinished);
}

// 绘图事件
void LrcWidget::paintEvent(QPaintEvent *event)
{
    // 重绘背景
    repaintBackground();
    QWidget::paintEvent(event);
}

// 设置背景为高斯模糊后的专辑图片
void LrcWidget::setGaussblurBackground(const QString &url)
{
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    manager->get(request);
}

// 重新绘制背景图
void LrcWidget::repaintBackground()
{
    // 背景图不为空
    if (img.isNull())
        return;
    // 绘制背景图片
    this->setAutoFillBackground(true);
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Window, QBrush(img.scaled(this->size())));
    this->setPalette(pal);
}

// 网络应答处理
void LrcWidget::onRequestFinished(QNetworkReply *reply)
{
    // 获取响应状态码，200表示正常
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<__FILE__<<__LINE__ << "状态码：" << status_code;

    if (reply->error() == QNetworkReply::NoError) {
        // 获取字节数据
        QByteArray bytes = reply->readAll();
        // 转换为图片
        QImage img;
        img.loadFromData(bytes);
        // 在线程中模糊背景图片
        QtConcurrent::run(gauss, GaussianBlur::gaussBlur, img, radius);
    }else {
        qDebug()<<__FILE__<<__LINE__<<"获取专辑图片失败";
    }
    reply->deleteLater();
}

// 高斯模糊图片显示
void LrcWidget::onGaussianFinished(QImage image)
{
    img = image;
    update();
}



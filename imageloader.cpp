#include "imageloader.h"
#include <QPixmap>

ImageLoader::ImageLoader(QObject *parent)
    : QObject{parent}
{
    m_networkAccessManager = new QNetworkAccessManager(this);
    m_cache.setMaxCost(100);
}

// 加载图片
void ImageLoader::loadImage(QString url)
{
    if (m_cache.contains(url)) {
        // 如果图片已经缓存，则直接从缓存中获取
        emit imageLoaded(url, m_cache.object(url));
        return;
    }
    QNetworkRequest request(url);
    QNetworkReply *reply = m_networkAccessManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, url, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 下载成功，缓存图片并发出信号
            m_cache.insert(url, reply);
            emit imageLoaded(url, reply);
        } else {
            // 下载失败，发出错误信号
            emit loadError(url, reply->errorString());
        }
        reply->deleteLater();
    });
}

ImageLoaderThread::ImageLoaderThread(ImageLoader *imageLoader, const QString &url)
    : m_imageLoader(imageLoader), m_url(url)
{

}

void ImageLoaderThread::run()
{
    m_imageLoader->loadImage(m_url);
}

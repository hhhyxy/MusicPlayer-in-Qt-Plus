#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QCache>
#include <QRunnable>
#include <QSharedPointer>

class ImageLoader : public QObject
{
    Q_OBJECT
public:
    explicit ImageLoader(QObject *parent = nullptr);
    void loadImage(QString url);
signals:
    void imageLoaded(QString url, QNetworkReply* reply);
    void loadError(const QString &url, const QString &errorString);

private:
    QNetworkAccessManager *m_networkAccessManager;
    QCache<QString, QNetworkReply> m_cache;
};

class ImageLoaderThread : public QRunnable
{
public:
    explicit ImageLoaderThread(ImageLoader *imageLoader, const QString &url);
    void run();
private:
    ImageLoader *m_imageLoader;
    QString m_url;
};

#endif // IMAGELOADER_H

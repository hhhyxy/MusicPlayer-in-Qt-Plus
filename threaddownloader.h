#ifndef THREADDOWNLOADER_H
#define THREADDOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>

class ThreadDownLoader : public QObject
{
    Q_OBJECT
public:
    explicit ThreadDownLoader(QObject *parent = nullptr);
    // 下载
    void downLoad(QString &url);
    // 多线程分段下载
    void multiDownLoad(QString &url, qint64 totalBytes);
signals:
    // 下载完成信号
    void finished(QByteArray bytes);
private:
    QNetworkAccessManager *manager;     // 网络访问管理
    QVector<QNetworkReply*> replies;    // 每一个段的网络应答
    QNetworkReply *headerReply;         // 网络响应头的应答
    int numThreads;     // 线程数
    int threadFinshed;  // 已完成的请求数
    QString url;        // 专辑图片链接
private slots:
    // 响应头返回
    void onHeaderReplyFinished();
    // 某一段响应返回
    void onReplyFinished();
private:

};

#endif // THREADDOWNLOADER_H

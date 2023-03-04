#include "threaddownloader.h"
#include <QNetworkRequest>

ThreadDownLoader::ThreadDownLoader(QObject *parent)
    : QObject{parent}
{
    numThreads = 1;
    manager = new QNetworkAccessManager(this);

}

void ThreadDownLoader::downLoad(QString &url)
{
    threadFinshed = 0;
    this->url = url;
    // 发送请求获取响应头
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    headerReply = manager->head(request);
    connect(headerReply, QNetworkReply::finished, this, ThreadDownLoader::onHeaderReplyFinished, Qt::UniqueConnection);
}

void ThreadDownLoader::onHeaderReplyFinished()
{
    // 获取响应状态码，200表示正常
    int status_code = headerReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<__FILE__<<__LINE__ << "状态码：" << status_code;
    if (headerReply->error() == QNetworkReply::NoError) {
        // 获取专辑图片大小
        qint64 totalBytes = headerReply->header(QNetworkRequest::ContentLengthHeader).toLongLong();
        qDebug()<< "totalBytes:" <<totalBytes;
        // 分段下载
        multiDownLoad(url, totalBytes);
    } else {
        qDebug()<<__FILE__<<__LINE__<<"获取响应头失败";
        // 内存释放
        headerReply->deleteLater();
        headerReply = nullptr;
    }
}

void ThreadDownLoader::multiDownLoad(QString &url, qint64 totalBytes)
{
    int mb = totalBytes /1000000;
    if (mb > 9)
        numThreads = 3;
    else
        numThreads = 2;

    // 根据线程数分段，计算出每一段的大小
    qint64 partSize = totalBytes / numThreads;
    qDebug()<<"partSize:"<<partSize;
    // 计算出每一段的开始和结束位置，存储在parts中
    QVector<QPair<qint64, qint64>> parts;
    qint64 startByte = 0;
    qint64 endByte = -1;
    for (int i = 0; i < numThreads; i++) {
        startByte = endByte + 1;
        endByte = startByte + partSize - 1;
        if (i == numThreads - 1) {
            endByte = totalBytes - 1;
        }
        parts.append(qMakePair(startByte, endByte));
        qDebug()<<"index:"<<i<<"start:"<<startByte<<"end:"<<endByte;
    }
    // 分段发送请求
    for (int i = 0; i < numThreads; i++) {
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        request.setRawHeader("Range", QString("bytes=%1-%2").arg(parts[i].first).arg(parts[i].second).toLatin1());
        QNetworkReply *reply = manager->get(request);
        replies.push_back(reply);
        connect(reply, &QNetworkReply::finished, this, &ThreadDownLoader::onReplyFinished);
    }
}

void ThreadDownLoader::onReplyFinished()
{
    // 等待所有请求完成
    threadFinshed++;
    if (threadFinshed < numThreads)
        return;
    // 合并每一段的数据
    QByteArray bytes;
    for (int i = 0, length = replies.size(); i < length; i++) {
        QNetworkReply *reply = replies.at(i);
        bytes.append(reply->readAll());
        reply->deleteLater();
    }
    // 发送完成信号
    emit finished(bytes);
    qDebug()<<"MultiDownLoad finished";
}



#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include "threaddownloader.h"
/*
 * 用于图片显示的QLabel
 */
class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);
    ~MyLabel();
    // 通过专辑图片链接设置圆角图片
    void setRadiusPixmap(QString url);
    // 获取专辑图片
    QPixmap getImg() const;
    void setShadow();
private:
    QNetworkAccessManager   *networkManager = nullptr;  // 网络访问管理
    QNetworkRequest         *request = nullptr;         // 网络请求
    ThreadDownLoader        *downLoader;                // 多线程下载器
    QPixmap img;                    // 专辑图片
    QString picUrl = "";            // 专辑图片链接
    bool imgLoaded = false;                 // 图片是否加载完成
private:
    // 显示圆角图片
    void makeRadiusPixmap(QByteArray bytes);
private slots:
    // 请求完成处理函数
    void onReplyFinished(QNetworkReply *reply);
    // 多线程下载完成
    void onMultiDownLoadFinished(QByteArray bytes);
};

#endif // MYLABEL_H

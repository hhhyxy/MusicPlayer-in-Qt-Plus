#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <mutex>
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
    // 通过专辑图片链接设置圆角图片(多线程)
//    void setRoundedPicInThread(QString url);
    // 获取专辑图片
    QPixmap getImg() const;
    // 显示圆角图片
    void makeRadiusPixmap(QByteArray bytes);
    void makeRadiusBlurPixmap(QByteArray bytes);
    void makeRadiusPixmap(QPixmap pixmap);
    // 设置图片阴影
    void setShadow();
    void setBackRole(int show = true);
private:
    QNetworkAccessManager   *networkManager = nullptr;  // 网络访问管理
    QNetworkRequest         *request = nullptr;         // 网络请求
//    ThreadDownLoader        *downLoader;                // 多线程下载器
    QString picUrl = "";            // 专辑图片链接
    bool imgLoaded = false;         // 图片是否加载完成
    std::mutex mutex;
    bool backRole = false;
    int radius = 0;
private slots:
    // 请求完成处理函数
    void onReplyFinished(QNetworkReply *reply);
};

#endif // MYLABEL_H

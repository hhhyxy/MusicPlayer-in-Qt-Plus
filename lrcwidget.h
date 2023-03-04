#ifndef LRCWIDGET_H
#define LRCWIDGET_H

#include <QWidget>
#include <Qpainter>
#include <QNetworkAccessManager>
#include "gaussianblur.h"

/*
 * 歌词界面
 */
class LrcWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LrcWidget(QWidget *parent = nullptr);
    // 设置高斯模糊背景图片
    void setGaussblurBackground(const QString &url);
protected:
    // 绘图事件
    void paintEvent(QPaintEvent *event);
private slots:
    // 请求完成
    void onRequestFinished(QNetworkReply *reply);
    // 图片模糊处理完成
    void onGaussianFinished(QImage image);
private:
    QNetworkAccessManager   *manager;   // 网络请求管理
    GaussianBlur            *gauss;     // 图片高斯模糊类
    int radius; // 模糊半径
    QImage img; // 背景图片
private:
    // 重绘背景图
    void repaintBackground();
};

#endif // LRCWIDGET_H

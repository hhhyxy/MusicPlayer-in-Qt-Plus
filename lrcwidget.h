#ifndef LRCWIDGET_H
#define LRCWIDGET_H

#include <QWidget>
#include <Qpainter>
#include <QNetworkAccessManager>
#include "gaussianblur.h"

class LrcWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LrcWidget(QWidget *parent = nullptr);

    void setGaussblurBackground(const QString &url);
    void repaintBackground();
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    void onRequestFinished(QNetworkReply *reply);
    void onGaussianFinished(QImage image);
private:
    QNetworkAccessManager *manager;
    GaussianBlur *gauss;
    QImage img;
};

#endif // LRCWIDGET_H

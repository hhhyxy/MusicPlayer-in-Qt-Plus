#ifndef GAUSSIANBLUR_H
#define GAUSSIANBLUR_H

#include <QObject>
#include <QImage>

/*
 * QImage图片高斯模糊处理
 */
class GaussianBlur : public QObject
{
    Q_OBJECT
public:
    explicit GaussianBlur(QObject *parent = nullptr);
    // 图片高斯模糊
    void gaussBlur(QImage &img, int radius);
signals:
    // 图片处理完成信号
    void finished(QImage image);
private:
    void gaussBlur1(int* pix, int w, int h, int radius);
    void gaussBlur2(int* pix, int w, int h, int radius);
    void boxBlurH(int* srcPix, int* destPix, int w, int h, int radius);
    void boxBlurV(int* srcPix, int* destPix, int w, int h, int radius);
    void boxBlur(int* srcPix, int* destPix, int w, int h, int r);
    void boxesForGauss(float sigma, int* size, int n);
};

#endif // GAUSSIANBLUR_H

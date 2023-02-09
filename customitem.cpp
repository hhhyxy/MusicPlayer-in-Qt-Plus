#include "customitem.h"
#include "ui_customitem.h"
#include <QTime>
#include <QPainter>
#include <QPainterPath>
#include <QBitmap>
#include <QMouseEvent>
#include <QStyleOption>

CustomItem::CustomItem(Music music, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomItem)
{
    ui->setupUi(this);
    ui->label_albumPic->setFixedSize(60, 60);
    setWindowFlags(Qt::WindowTransparentForInput | Qt::FramelessWindowHint);//窗口仅用于输出，不接收任何输入事件
    this->music = music;
    // 初始化网络请求
    networkManager = new QNetworkAccessManager(this);
    request = new QNetworkRequest();
    connect(networkManager, &QNetworkAccessManager::finished, this, &CustomItem::replyFinished);
    setData();
}

CustomItem::~CustomItem()
{
    delete ui;
    delete request;
    request = nullptr;
}

void CustomItem::setData()
{
    // 显示专辑图片
    showAlbumPic();
    // 显示歌名、歌手、专辑名、时长
    qDebug()<<__FILE__<<__LINE__ << tr("songName:") << music.getSongName();
    ui->label_songName->setText(music.getSongName());
    ui->label_author->setText(music.getAuthor());
    ui->label_albumName->setText(music.getAlbumName());
    ui->label_duration->setText(music.getSongDuration());
}

bool CustomItem::albumPicLoadingIsFinished() const
{
    return albumPicLoadingFinished;
}

void CustomItem::changeFontColor(QString color)
{
    QString qss = QString("color:%1").arg(color);
    this->setStyleSheet(qss);
}

Music CustomItem::getMusic() const
{
    return music;
}

// 圆角图片
QPixmap CustomItem::image2Radius(QPixmap pixmap, int radius)
{
    //获取图片尺寸
    int imageWidth = pixmap.width();
    int imageHeight = pixmap.height();

    //处理大尺寸的图片,保证图片显示区域完整
    QPixmap newPixMap = pixmap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight),Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QPixmap destImage(imageWidth, imageHeight);
    destImage.fill(Qt::transparent);
    QPainter painter(&destImage);
    // 抗锯齿
    painter.setRenderHints(QPainter::Antialiasing, true);
    // 图片平滑处理
    painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
    // 将图片裁剪为圆角
    QPainterPath path;
    if (radius ==0) {
        radius = imageWidth * 0.1;
    }
    QRect rect(0, 0, imageWidth, imageHeight);
    path.addRoundedRect(rect, radius, radius);
    painter.setClipPath(path);
    painter.drawPixmap(0, 0, imageWidth, imageHeight, newPixMap);
    return destImage;
}

void CustomItem::showAlbumPic()
{
    request->setUrl(QUrl(music.albumPicUrl()));
    networkManager->get(*request);

}

void CustomItem::replyFinished(QNetworkReply *reply)
{
    // 获取响应状态码，200表示正常
    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug()<<__FILE__<<__LINE__ << tr("状态码：") << status_code;

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray bytes = reply->readAll();  //获取字节
        // 加载图片
        QPixmap pixmap;
        pixmap.loadFromData(bytes);
        music.setAlbumPic(image2Radius(pixmap));
        albumPicLoadingFinished = true;
        ui->label_albumPic->setPixmap(music.albumPic());

    }else {
        qDebug()<<__FILE__<<__LINE__<<"获取专辑图片失败";
    }
    reply->deleteLater();
    reply = nullptr;
}

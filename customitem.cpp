#include "customitem.h"
#include "ui_customitem.h"
#include <QTime>
#include <QTimer>
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
    ui->label_albumPic->setFixedSize(70, 70);
    setWindowFlags(Qt::FramelessWindowHint);//窗口仅用于输出，不接收任何输入事件
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
    QString qss = QString("*{color:%1}").arg(color);
    this->setStyleSheet(qss);
}

Music CustomItem::getMusic() const
{
    return music;
}

// 圆角图片
QPixmap CustomItem::image2Radius(QPixmap img, int radius)
{
    if (img.isNull())
    {
        qDebug()<<__FILE__<<__LINE__<<"img is null";
        return QPixmap();
    }
    //处理大尺寸的图片
    if (img.width() > 700) {
        img = img.scaledToWidth(700);
    }

    QSize size = img.size();
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    if (radius == 0) {
        radius = size.width() * 0.07;
    }
    painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius);//修改这个值，可以改弧度，和直径相等就是圆形
    img.setMask(mask);
    return img;
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
        ui->label_albumPic->setPixmap(music.albumPic().scaled(ui->label_albumPic->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    }else {
        qDebug()<<__FILE__<<__LINE__<<"获取专辑图片失败";
        QTimer::singleShot(1000, this, CustomItem::showAlbumPic);
    }
    reply->deleteLater();
    reply = nullptr;
}

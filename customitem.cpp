#include "customitem.h"
#include "ui_customitem.h"
#include "haomusic.h"

#include <QBitmap>
#include <QImage>
#include <QTime>
#include <QTimer>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QStyleOption>
#include <QGraphicsEffect>

CustomItem::CustomItem(Music music, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomItem)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    this->music = music;
    connect(this, CustomItem::customContextMenuRequested, this, CustomItem::showMenu);
    QString songName = ui->label_songName->fontMetrics().elidedText(music.getSongName(), Qt::ElideRight, width()*0.4);
    QString author    = ui->label_author->fontMetrics().elidedText(music.getAuthor(), Qt::ElideRight, width()*0.4);
    QString albumName = ui->label_songName->fontMetrics().elidedText(music.getAlbumName(), Qt::ElideRight, width()*0.4);
    ui->label_songName->setText(songName);
    ui->label_author->setText(author);
    ui->label_albumName->setText(albumName);
    ui->label_duration->setText(music.getSongDuration());
    ui->label_albumPic->setRadiusPixmap(music.albumPicUrl());
}

CustomItem::~CustomItem()
{
    delete ui;
}

// 改变字体颜色
void CustomItem::changeFontColor(QString color)
{
    QString qss = QString("color:%1;").arg(color);
    this->setStyleSheet(qss);
}

QPixmap CustomItem::getAlbumPic()
{
    return ui->label_albumPic->getImg();
}

void CustomItem::setItemType(int itemType)
{
    this->itemType = itemType;
}

void CustomItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button()) {
        emit myItemDoubleClicked(this);
        return;
    }
    QWidget::mouseDoubleClickEvent(event);
}

int CustomItem::getItemType() const
{
    return itemType;
}

Music CustomItem::getMusic() const
{
    return music;
}

// 初始化菜单
void CustomItem::initMenu()
{
    menu = new QMenu(this);
    menu->addAction("播放音乐", [=] {
        emit musicPlay(this);
    });
    if (itemType == CustomItem::FAVORITE) {
        menu->addAction("添加到歌单", [=] {
            emit addToSonglist(this);
        });
        menu->addAction("从我喜欢的音乐中删除", [=] {
            emit addToSonglist(this);
        });
    }
    menu->addAction("添加到我喜欢的音乐", [=] {
        emit addToMyFavoriteMusic(this);
    });
    menu->addAction("添加到歌单", [=] {
        emit addToSonglist(this);
    });
    menu->setWindowFlags(menu->windowFlags()  | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0,0);
    effect->setColor(QColor(150,150,150));
    effect->setBlurRadius(10);
    menu->setGraphicsEffect(effect);
}

// 显示右键菜单
void CustomItem::showMenu()
{
    initMenu();
    menu->exec(QCursor::pos());
    menu->deleteLater();
    menu = nullptr;
}


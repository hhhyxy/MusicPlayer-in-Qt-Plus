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
    // 无边框窗体
    setWindowFlags(Qt::FramelessWindowHint);
    // 初始化音乐信息
    this->music = music;
    // 字体溢出显示省略号
    QString songName = ui->label_songName->fontMetrics().elidedText(music.getSongName(), Qt::ElideRight, width()*0.4);
    QString author    = ui->label_author->fontMetrics().elidedText(music.getAuthor(), Qt::ElideRight, width()*0.4);
    QString albumName = ui->label_songName->fontMetrics().elidedText(music.getAlbumName(), Qt::ElideRight, width()*0.4);
    ui->label_songName->setText(songName);
    ui->label_author->setText(author);
    ui->label_albumName->setText(albumName);
    ui->label_duration->setText(music.getSongDuration());
    // 显示专辑图片
    ui->label_albumPic->setRadiusPixmap(music.albumPicUrl());
    // 自定义菜单
    connect(this, CustomItem::customContextMenuRequested, this, CustomItem::showMenu);
}

CustomItem::~CustomItem()
{
    delete ui;
}

// 鼠标双击事件
void CustomItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (Qt::LeftButton == event->button()) {
        emit myItemDoubleClicked(this);
        return;
    }
    QWidget::mouseDoubleClickEvent(event);
}

// 获取专辑图片
QPixmap CustomItem::getAlbumPic()
{
    return ui->label_albumPic->getImg();
}

// 设置item类型
void CustomItem::setItemType(int itemType)
{
    this->itemType = itemType;
}

// 获取item类型
int CustomItem::getItemType() const
{
    return itemType;
}

// 获取item对应的音乐
Music CustomItem::getMusic() const
{
    return music;
}

// 改变字体颜色
void CustomItem::changeFontColor(QString color)
{
    QString qss = QString("color:%1;").arg(color);
    this->setStyleSheet(qss);
}

// 初始化菜单
void CustomItem::initMenu()
{
    // 初始化菜单
    menu = new QMenu(this);
    // 给菜单添加子选项
    menu->addAction("播放音乐", [=] {
        emit menuClicked(this, CustomItem::PLAY);
    });
    if (itemType == CustomItem::FAVORITE) {
        menu->addAction("从我喜欢的音乐中删除", [=] {
            emit menuClicked(this, CustomItem::REMOVEFROMFAVORITE);
        });
    } else {
        menu->addAction("添加到我喜欢的音乐", [=] {
            emit menuClicked(this, CustomItem::ADDTOFAVORITE);
        });
    }
    menu->addAction("添加到歌单", [=] {
        emit menuClicked(this, CustomItem::ADDTOSONGLIST);
    });
    // 菜单属性设置为无边框无阴影透明背景
    menu->setWindowFlags(menu->windowFlags()  | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    // 给菜单设置阴影
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0,0);
    effect->setColor(QColor(150,150,150));
    effect->setBlurRadius(10);
    menu->setGraphicsEffect(effect);
}

// 显示右键菜单
void CustomItem::showMenu()
{
    // 初始化菜单
    initMenu();
    // 菜单显示在鼠标点击的位置
    menu->exec(QCursor::pos());
    menu->deleteLater();
    menu = nullptr;
}

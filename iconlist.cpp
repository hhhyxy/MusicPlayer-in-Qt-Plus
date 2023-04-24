#include "iconlist.h"
#include "musicdb.h"

#include <QDebug>
#include <QPixmap>
#include <QPainter>

IconList::IconList(QWidget *parent)
    : QListWidget{parent}
{
    iconSize = 250;
    wordHight = 30;
    this->setViewMode(QListWidget::IconMode);       // 显示模式
    this->setSpacing(30);                           // 间距
    this->setIconSize(QSize(iconSize, iconSize));   // 设置图片大小
    this->setMovement(QListView::Static);           // 不能移动
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void IconList::init()
{
    this->clear();
    addIcons(MusicDB::instance()->queryList());
}

void IconList::addIcons(QMap<int, QString> map)
{
    QMapIterator<int, QString> iter(map);
    while (iter.hasNext()) {
        iter.next();
        addIcon(iter.key(), iter.value());
    }
}

void IconList::addIcon(int id, QString name)
{
    // 绘制背景文字图
    QPixmap img(":/icon/rounded .svg");
//    img.scaledToWidth(iconSize);
    QPainter painter(&img);
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPixelSize(20);
    font.setBold(true);
//    font.setLetterSpacing(QFont::PercentageSpacing, 120);
    font.setFamily("Microsoft YaHei");
    painter.setFont(font);
    painter.drawText(img.rect(), Qt::AlignCenter, name);

    QListWidgetItem *imageItem = new QListWidgetItem();
    imageItem->setSizeHint(QSize(iconSize, iconSize));
    imageItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    imageItem->setIcon(QIcon(img.scaled(imageItem->sizeHint())));
    imageItem->setData(Qt::UserRole, id);
    imageItem->setData(Qt::UserRole + 1, name);
    this->addItem(imageItem);
}

void IconList::paintEvent(QPaintEvent *e)
{
    int width = this->width();  // listwidget的宽度
    int num;
    if (width < 1500)
        num = 4;
    else
        num = 5;
    int spacing = (width - (num * iconSize)) / num - num * num;
    this->setSpacing(spacing);
    QListWidget::paintEvent(e);
}

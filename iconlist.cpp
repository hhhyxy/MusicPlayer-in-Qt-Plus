#include "iconlist.h"
#include "musicdb.h"
#include "coveritem.h"

#include <QContextMenuEvent>
#include <QDebug>
#include <QGraphicsDropShadowEffect>
#include <QListWidgetItem>
#include <QPixmap>
#include <QPainter>

IconList::IconList(QWidget *parent)
    : QListWidget{parent}
{
    iconSize = 210;
    wordHight = 30;
    margin = 20;
    this->setViewMode(QListWidget::IconMode);       // 显示模式
    this->setSpacing(30);                           // 间距
    this->setIconSize(QSize(iconSize + 2 *margin, iconSize + 2 * margin));   // 设置图片大小
    this->setMovement(QListView::Static);           // 不能移动
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //listWidget属性设置为自定义菜单
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &IconList::customContextMenuRequested, this, &IconList::onCustomContextMenuRequested);
}

void IconList::init()
{
    this->clear();
    addIcons(MusicDB::instance()->queryList());
}

void IconList::addIcons(QList<MusicList> lists)
{
    foreach (MusicList list, lists) {
        addIcon(list);
    }
}

void IconList::addIcon(MusicList& list)
{
    QListWidgetItem *imageItem = new QListWidgetItem();
    imageItem->setSizeHint(QSize(iconSize + 2 *margin, iconSize + 2* margin + wordHight));
    imageItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    imageItem->setData(Qt::UserRole, list.id());
    imageItem->setData(Qt::UserRole + 1, list.name());


    CoverItem* item = new CoverItem();
    item->setMargin(margin);
    item->resize(imageItem->sizeHint());
    item->setCoverSize(QSize(iconSize, iconSize));
    item->setCover(list.cover());
    item->setName(list.name());

    this->addItem(imageItem);
    this->setItemWidget(imageItem, item);
}

void IconList::resizeEvent(QResizeEvent *e)
{
    int width = this->width();  // listwidget的宽度
    int num;
    if (width < 1500)
        num = 4;
    else
        num = 5;
    int spacing = (width - (num * (iconSize + 2 *margin))) / num - num * num;
    this->setSpacing(spacing);
    QListWidget::resizeEvent(e);
}

void IconList::dropSongsList(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();
    if (id <= MyListWidget::DEFALUT)
        return;
    delete this->takeItem(this->row(item));
    MusicDB::instance()->removeList(id);
    init();
}

void IconList::renameSongsList(QListWidgetItem *item)
{

}

void IconList::creatSongsList()
{
    emit create();
}

void IconList::onCustomContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    QMenu* menu = new QMenu(this);
    // 菜单属性设置为无边框无阴影透明背景
    menu->setWindowFlags(menu->windowFlags()  | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    // 给菜单设置阴影
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setOffset(0,0);
    effect->setColor(QColor(150,150,150));
    effect->setBlurRadius(10);
    menu->setGraphicsEffect(effect);

    QAction* create = new QAction("新建歌单", menu);
    connect(create, &QAction::triggered, this, [=] {
        creatSongsList();
    });
    menu->addAction(create);

    QListWidgetItem *item = this->itemAt(mapFromGlobal(QCursor::pos()));
    if (item != nullptr) {
        QAction * rename = new QAction("重命名", menu);
        QAction * drop = new QAction("删除歌单", menu);
        menu->addAction(rename);
        menu->addAction(drop);
        connect(rename, &QAction::triggered, [=] {
            renameSongsList(item);
        });
        connect(drop, &QAction::triggered, [=] {
            dropSongsList(item);
        });
    }
    qDebug() << "menu";
    menu->exec(QCursor::pos());
}

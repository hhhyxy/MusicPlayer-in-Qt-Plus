#include "mylistwidget.h"
#include <QtConcurrent>

MyListWidget::MyListWidget(QWidget *parent)
    : QListWidget{parent}
{

}

CustomItem * MyListWidget::addCustomItem(Music music)
{
    QListWidgetItem* qItem = new QListWidgetItem(this);
    this->addItem(qItem);
    qItem->setSizeHint(QSize(0, 80));

    CustomItem *myItem = new CustomItem(music, this);
    myItem->setItemType(CustomItem::FAVORITE);
//    QtConcurrent::run(myItem, CustomItem::showInfo);
//    myItem->showInfo();
    this->setItemWidget(qItem, myItem);
    connect(myItem, &CustomItem::myItemDoubleClicked, this, MyListWidget::customItemDoubleClicked);
    return myItem;
}

QList<CustomItem *> MyListWidget::addCustomItems(QList<Music> musicList)
{
    this->clear();
    this->scrollToTop();
    QList<CustomItem *> customItems;
    foreach (Music music, musicList) {
        customItems.push_back(addCustomItem(music));
    }
    return customItems;
}

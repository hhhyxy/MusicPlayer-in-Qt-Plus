#include "mylistwidget.h"
#include <QScrollBar>
#include <QThreadPool>
#include <QDebug>
MyListWidget::MyListWidget(QWidget *parent)
    : QListWidget{parent}
{
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &MyListWidget::onScrollBarValueChange);
}

void MyListWidget::init(int id, QString name)
{
    this->id = id;
    this->name = name;
}

// 设置音乐列表，并加载前10项
void MyListWidget::setMusicList(const QList<Music> &musicList)
{
    // 清空列表并滚动到顶部
    items.clear();
    this->clearList();
    loadmore = true;
    this->scrollToTop();
    // 更新音乐列表，加载前10项
    m_musicList.clear();
    m_musicList = musicList;
    itemsNum = 10;
    if (itemsNum > musicList.size())
        itemsNum = musicList.size();
    addCustomItems(0, itemsNum);
    emit loaded(id);
}

// 插入item
void MyListWidget::insertCustomItem(Music music, int row)
{
    // 如果插入位置正常
    if (row >= 0 && row < itemsNum)
        addCustomItem(music, row);
}

void MyListWidget::removeCustomItem(CustomItem *item)
{
    int index = items.indexOf(item);
    items.removeAt(index);
    QListWidgetItem *lItem =  this->takeItem(index);
    delete lItem;
    item = nullptr;
}

int MyListWidget::getListType()
{
    return id;
}

// 当滚动到底部，加载更多项
void MyListWidget::onScrollBarValueChange(int value)
{
    if (loadmore && value == this->verticalScrollBar()->maximum()) {
        loadMore();
    }
}

// 加载更多
void MyListWidget::loadMore()
{
    // 当没有item可加载时返回
    if (itemsNum >= m_musicList.size())
        return;
    // 需加载的第一项
    int begin = itemsNum;
    // 需加载的最后一项
    int end = itemsNum + 4;
    // item数量不大于音乐数量
    if (end > m_musicList.size())
        end = m_musicList.size();
    // 添加item
    addCustomItems(begin, end);
    itemsNum = end;
    qDebug() << __FILE__ << __LINE__ << "load more" << begin << end;
}

// 清空列表
void MyListWidget::clearList()
{
    loadmore = false;
    this->clear();
    emit cleared(id);
}

void MyListWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QListWidget::mouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
}



// 加载items
void MyListWidget::addCustomItems(int begin, int end)
{
    for (int i = begin; i < end; i++) {
        Music music = m_musicList.at(i);
        addCustomItem(music);
    }
}

// 加载自定义item
void MyListWidget::addCustomItem(const Music music, int row/* = -1*/)
{
    QListWidgetItem* qItem = new QListWidgetItem();
    if (row == -1)
        this->addItem(qItem);
    else
        this->insertItem(row, qItem);
//    itemsNum++;
    qItem->setSizeHint(QSize(0, 80));

    CustomItem *myItem = new CustomItem(music);
    if (row == 0)
        items.push_front(myItem);
    else
        items.push_back(myItem);
    myItem->setItemType(id);
    this->setItemWidget(qItem, myItem);
    connect(myItem, &CustomItem::myItemDoubleClicked, this, &MyListWidget::customItemDoubleClicked);
    connect(myItem, &CustomItem::menuClicked, this, &MyListWidget::menuClicked);
}

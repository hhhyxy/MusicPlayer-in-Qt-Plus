#include "mylistwidget.h"
#include <QScrollBar>

MyListWidget::MyListWidget(QWidget *parent)
    : QListWidget{parent}
{
    connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &MyListWidget::onScrollBarValueChange);
}

// 设置音乐列表，并加载前15项
void MyListWidget::setMusicList(const QList<Music> &musicList)
{
    // 清空列表并滚动到顶部
    this->clear();
    this->scrollToTop();
    // 更新音乐列表，加载前15项
    m_musicList = musicList;
    itemsNum = 10;
    if (itemsNum > musicList.size())
        itemsNum = musicList.size();
    addCustomItems(0, itemsNum);
}

// 设置列表类型
void MyListWidget::setListType(int listType)
{
    this->listType = listType;
}

// 当滚动到底部，加载更多项
void MyListWidget::onScrollBarValueChange(int value)
{
    if (value == this->verticalScrollBar()->maximum()) {
        loadMore();
    }
}

// 加载更多
void MyListWidget::loadMore()
{
    // 当没有item可加载时返回
    if (itemsNum == m_musicList.size())
        return;
    // 需加载的第一项
    int begin = itemsNum;
    // 更新item数量
    itemsNum = itemsNum + 10;
    // item数量不大于音乐数量
    if (itemsNum > m_musicList.size())
        itemsNum = m_musicList.size();
    // 需加载的最后一项
    int end = itemsNum;
    // 添加item
    addCustomItems(begin, end);
}

// 加载items
void MyListWidget::addCustomItems(int begin, int end)
{
    for (int i = begin; i < end; i++) {
        addCustomItem(m_musicList.at(i));
    }
}

// 加载自定义item
void MyListWidget::addCustomItem(const Music music)
{
    QListWidgetItem* qItem = new QListWidgetItem(this);
    this->addItem(qItem);
    qItem->setSizeHint(QSize(0, 80));

    CustomItem *myItem = new CustomItem(music, this);
    myItem->setItemType(listType);
    this->setItemWidget(qItem, myItem);
    connect(myItem, &CustomItem::myItemDoubleClicked, this, &MyListWidget::customItemDoubleClicked);
    connect(myItem, &CustomItem::menuClicked, this, &MyListWidget::menuClicked);
}

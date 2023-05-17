#ifndef SEARCHTIPSLIST_H
#define SEARCHTIPSLIST_H

#include <QListWidget>

class SearchTipsList : public QListWidget
{
    Q_OBJECT
public:
    explicit SearchTipsList(QWidget *parent = nullptr);
    // 根据item数量改变列表大小
    void updateSize();
    // 显示搜索提示
    void showTips();
    // 隐藏搜索提示
    void hideTips();
protected:
    QSize sizeHint();
private:
    void animation(bool show = true);
private:
    int aniDuration;
};

#endif // SEARCHTIPSLIST_H

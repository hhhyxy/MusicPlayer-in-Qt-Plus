#include "haomusic.h"

#include <QApplication>
#include <QThreadPool>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HaoMusic w;
    w.setGeometry(253, 32, 1503, 970);
    w.setFixedSize(1503, 970);
    //去掉标题栏 点击任务栏图标显示/隐藏窗口
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    // 外层窗口透明
    w.setAttribute(Qt::WA_TranslucentBackground);
    QThreadPool::globalInstance()->setMaxThreadCount(20);	// 设置最大线程个数为30
    w.show();
    return a.exec();
}

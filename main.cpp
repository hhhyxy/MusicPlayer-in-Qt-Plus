#include "haomusic.h"

#include <QApplication>
#include <QThreadPool>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 连接数据库
    MusicDB::instance()->open();
    HaoMusic w;
    w.setGeometry(253, 32, 1503, 970);
    //去掉标题栏 点击任务栏图标显示/隐藏窗口
    w.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    // 外层窗口透明
    w.setAttribute(Qt::WA_TranslucentBackground);
    QThreadPool::globalInstance()->setMaxThreadCount(50);	// 设置最大线程个数为50
    w.show();
    return a.exec();
}

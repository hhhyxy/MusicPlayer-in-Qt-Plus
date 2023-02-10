#ifndef HAOMUSIC_H
#define HAOMUSIC_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QListWidgetItem>
#include <QSystemTrayIcon>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QPixmap>
#include <QSize>
#include <QTableWidgetItem>
#include "music.h"
#include "myhttp.h"
#include "customitem.h"


QT_BEGIN_NAMESPACE
namespace Ui { class HaoMusic; }
QT_END_NAMESPACE

class HaoMusic : public QWidget
{
    Q_OBJECT

public:
    HaoMusic(QWidget *parent = nullptr);
    ~HaoMusic();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void iconActived(QSystemTrayIcon::ActivationReason);

    void on_pushButton_close_clicked();

    void on_pushButton_maxsize_clicked();

    void on_pushButton_minsize_clicked();

    void on_pushButton_search_clicked();

    void on_listWidget_searchResult_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_lastsong_clicked();

    void on_pushButton_switch_clicked();

    void on_pushButton_nextsong_clicked();

    void on_pushButton_mode_clicked();

    void on_pushButton_volume_clicked();

private:
    Ui::HaoMusic    *ui;
    QMediaPlaylist  *mediaPlaylist      = nullptr;  // 媒体播放列表
    QMediaPlayer    *mediaPlayer        = nullptr;  // 媒体播放器
    MyHttp          *search             = nullptr;  // 网络搜索
    CustomItem      *currentPlayingItem = nullptr;  // 当前正在播放的item

    QPoint  m_mousePoint;       // 鼠标坐标
    QPoint  movePoint;          // 窗口移动距离
    bool    mousePress = false; // 鼠标左键是否按下

    int     volume          = 50;   // 音量
    int     currentLrcRow   = 1;    // 当前歌词所在行
    QString searchKeywords  = "";   // 搜索关键词
    QMovie *loadingMovie;           // 加载动画
    Music   currentMusic;           // 当前播放的音乐


    QListWidgetItem     *currentLrcItem;        // 当前歌词所在item
    QMap<int,QString>    lrcMap;                // 歌词
    QList<Music>         musicList;             // 播放列表音乐列表
    QList<Music>         searchResultMusicList; //搜索结果音乐列表

    // 向listwidget添加行
    void addItemToListWidget(Music music);
    // 设置托盘图标
    void setTrayIcon();
    // 初始化播放器
    void initPlayer();
    // 连接信号和槽
    void connectSignalsAndSlots();
    // 音乐进度条点击事件处理函数
    void sliderClicked();
    // 显示搜索结果
    void showSearchResult();
    // 更新播放列表
    void updateMediaPlaylist(QList<Music> musicList);
    // 更新底部栏音乐信息
    void updateBottomMusicInfo();
    // 更新布局
    void updateLayout();
    // 显示歌词
    void showLrc();
    // 歌词滚动
    void lrcRoll(int position);
    // 显示加载界面
    void showLoadingPage();
    // 更改当前播放项的样式
    void changeCurrentPlayingItem(CustomItem *item);
private slots:
    void onPositionChanged(qint64);
    void onDurationChanged(qint64);

    void on_horizontalSlider_volume_valueChanged(int value);
    void on_pushButton_recentlyplayed_clicked();
    void on_pushButton_defaultSongList_clicked();
    void on_pushButton_localmusic_clicked();
    void on_listWidget_lrc_itemClicked(QListWidgetItem *item);
    void on_pushButton_dropDown_clicked();
};
#endif // HAOMUSIC_H

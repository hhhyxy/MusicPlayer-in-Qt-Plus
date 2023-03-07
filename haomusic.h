#ifndef HAOMUSIC_H
#define HAOMUSIC_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <QListWidgetItem>
#include <QSystemTrayIcon>
#include <QAbstractButton>
#include <QUrl>
#include <QPixmap>
#include <QSize>

#include <QGraphicsDropShadowEffect>
#include "music.h"
#include "myhttp.h"
#include "customitem.h"
#include "musicdb.h"
#include "mymediaplaylist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HaoMusic; }
QT_END_NAMESPACE

/*
 * 主界面
 */
class HaoMusic : public QWidget
{
    Q_OBJECT

public:
    HaoMusic(QWidget *parent = nullptr);
    ~HaoMusic();
protected:
    // 鼠标按下事件
    void mousePressEvent(QMouseEvent *event);
    // 鼠标移动事件
    void mouseMoveEvent(QMouseEvent *event);
    // 鼠标释放事件
    void mouseReleaseEvent(QMouseEvent *event);
    // 鼠标双击事件
    void mouseDoubleClickEvent(QMouseEvent *event);
    // 绘图事件
    void paintEvent(QPaintEvent *event);
private:
    Ui::HaoMusic    *ui;
    MyMediaPlaylist  *mediaPlaylist      = nullptr;  // 媒体播放列表
    QMediaPlayer    *mediaPlayer        = nullptr;  // 媒体播放器
    MyHttp          *search             = nullptr;  // 网络搜索
    CustomItem      *currentPlayingItem = nullptr;  // 当前正在播放的item

    QPoint  m_mousePoint;       // 鼠标坐标
    QPoint  movePoint;          // 窗口移动距离
    bool    mousePress = false; // 鼠标左键是否按下

    int     listType = -1;      // 列表类型

    int     volume          = 30;   // 初始音量
    bool    isSearchResultUpdate    = false;    // 搜索结果是否更新
    bool    isFavoriteMusicListShow = false;    // 我喜欢的音乐列表是否显示
    bool    isLocalMusicListShow    = false;    // 本地音乐列表是否显示
    bool    isSonglistShow          = false;    // 我的歌单是否显示

    int     loadingTimes    = 1000; // 加载所需时间（ms）
    QString searchKeywords  = "";   // 搜索关键词
    QMovie *loadingMovie;           // 加载动画

    Music                currentMusic;          // 当前播放的音乐
    int                  currentLrcRow   = 1;   // 当前歌词所在行
    QListWidgetItem     *currentLrcItem;        // 当前歌词所在item
    QMap<int,QString>    lrcMap;                // 歌词
    QList<int>           lrcKeys;               // 歌词对应的时间帧
    QList<Music>         musicList;             // 播放列表音乐列表
    QList<Music>         searchResultMusicList; // 搜索结果音乐列表
    QList<Music>         favoriteMusicList;     // 我喜欢的音乐列表
    QList<Music>         localMusicList;        // 本地音乐列表
    MusicDB m_db;   // 音乐数据库
    // 绘制圆角阴影窗口
    void paintShadowRadiusWidget();
    // 设置托盘图标
    void setTrayIcon();
    // 初始化播放器
    void initPlayer();
    // 初始化音乐列表
    void initMusicList();
    // 连接信号和槽
    void connectSignalsAndSlots();
    // 音乐进度条点击事件处理函数
    void sliderClicked();
    // 显示搜索结果
    void showSearchResult();
    // 更新播放列表
    void updateMediaPlaylist();
    // 更新底部栏音乐信息
    void updateBottomMusicInfo();
    // 显示歌词
    void showLrc();
    // 歌词滚动
    void lrcRoll(int position);
    // 显示加载界面
    void showLoadingPage();
    // 更改当前播放项的样式
    void changeCurrentPlayingItem(CustomItem *item);
    // 播放当前项
    void playingTheItem(CustomItem *item);
    // 显示歌词页面
    void showLrcPage();
    // 显示所有我喜欢的音乐
    void showMyFavoriteMusicList();
    // 双击列表项
    void onCustomItemDoubleClicked(CustomItem *item);

private slots:
    // 双击托盘图标显示界面
    void iconActived(QSystemTrayIcon::ActivationReason);
    // 关闭窗口
    void on_pushButton_close_clicked();
    // 窗口最大化
    void on_pushButton_maxsize_clicked();
    // 窗口最小化
    void on_pushButton_minsize_clicked();
    // 点击搜索按钮
    void on_pushButton_search_clicked();
    // 点击上一首
    void on_pushButton_lastsong_clicked();
    // 点击播放按钮
    void on_pushButton_switch_clicked();
    // 点击下一首
    void on_pushButton_nextsong_clicked();
    // 点击播放模式切换按钮
    void on_pushButton_mode_clicked();
    // 点击静音按钮
    void on_pushButton_volume_clicked();
    // 正在播放音乐的当前时刻变化处理函数
    void onPositionChanged(qint64);
    // 音量条变化处理函数
    void on_horizontalSlider_volume_valueChanged(int value);
    // 点击历史播放按钮
    void on_pushButton_recentlyplayed_clicked();
    // 点击默认歌单按钮
    void on_pushButton_defaultSongList_clicked();
    // 点击本地音乐按钮
    void on_pushButton_localmusic_clicked();
    // 点击歌词界面下拉按钮
    void on_pushButton_dropDown_clicked();
    // 点击我喜欢的音乐按钮
    void on_pushButton_favorite_clicked();
    // 右键菜单点击事件
    void menuPlayMusicClicked(CustomItem *item);
    void menuAddToMyFavoriteClicked(CustomItem *item);
    void menuAddToSonglistClicked(CustomItem *item);
    // 点击列表的某一行
    void on_listWidget_lrc_itemClicked(QListWidgetItem *item);
    // 搜索框内容变化处理函数
    void on_lineEdit_search_textChanged(const QString &arg1);
    // 菜单点击事件处理函数
    void onMenuClicked(CustomItem *item, int itemType);
};

#endif // HAOMUSIC_H

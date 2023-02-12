#include "haomusic.h"
#include "ui_haomusic.h"
#include "mybottombar.h"
#include "mylineedit.h"

#include <QScroller>
#include <QScrollBar>
#include <QFocusEvent>
#include <QMovie>
#include <QTimer>
#include <QMenu>
#include <QPainter>

HaoMusic::HaoMusic(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HaoMusic)
{
    ui->setupUi(this);

    volume = 50;// 初始化音量

    // 设置托盘图标
    setTrayIcon();
    // 初始化播放器
    initPlayer();
    // 连接信号和槽
    connectSignalsAndSlots();
}

HaoMusic::~HaoMusic()
{
    delete ui;
}

// 初始化
void HaoMusic::initPlayer()
{
    paintShadowRadiusWidget();
    // 网络处理类初始化
    search = new MyHttp(this);
    // 播放器初始化
    mediaPlayer = new QMediaPlayer(this);
    mediaPlaylist = new QMediaPlaylist(this);
    mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);//设置播放模式(顺序播放，单曲循环，随机播放等)
    mediaPlayer->setPlaylist(mediaPlaylist);
    mediaPlayer->setVolume(volume);
    ui->horizontalSlider_volume->setValue(volume);
    // 加载动画
    loadingMovie = new QMovie(":/icon/loading.gif");
    ui->label_loading->setMovie(loadingMovie);
}

// 设置托盘图标
void HaoMusic::setTrayIcon()
{
    //新建一个托盘图标对象 在QSystemTrayIcon()中添加this指针指向musicplayer，以便在关闭窗口时销毁托盘图标
    QSystemTrayIcon *trayicon = new QSystemTrayIcon(this);
    // 设置托盘图标
    trayicon->setIcon(QIcon(QPixmap(":/icon/MusicPlayer.ico")));
    //设置托盘图标提示：鼠标移动到上面会提示文字
    trayicon->setToolTip(QString("MusicPlayer"));
    connect(trayicon, &QSystemTrayIcon::activated, this, &HaoMusic::iconActived);
    //创建菜单项
    QMenu *traymenu = new QMenu();
    //设置托盘图标右键菜单
    trayicon->setContextMenu(traymenu);
    // 设置托盘图标菜单
    traymenu->addAction("打开主界面",this,&HaoMusic::show);
    traymenu->addAction("退出",this,&HaoMusic::close);
    // 显示托盘图标
    trayicon->show();
}

// 双击托盘图标显示窗口
void HaoMusic::iconActived(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        //双击托盘显示窗口
        case QSystemTrayIcon::Trigger:
            this->setWindowState(Qt::WindowActive);
            this->show();
            break;
        default:
            break;
    }
}

// 连接信号和槽
void HaoMusic::connectSignalsAndSlots()
{
    // 绑定音乐进度条点击事件
    connect(ui->horizontalSlider_musicProgress,&CustomSlider::customSliderClicked,this,&HaoMusic::sliderClicked);

    // 绑定播放进度改变事件和总时长变化事件
    connect(mediaPlayer,&QMediaPlayer::positionChanged,this,&HaoMusic::onPositionChanged);
    connect(mediaPlayer,&QMediaPlayer::durationChanged,this,&HaoMusic::onDurationChanged);
    // 按Enter键搜索
    connect(ui->lineEdit_search, &QLineEdit::returnPressed, this, &HaoMusic::on_pushButton_search_clicked);
    // 播放状态改变，更新播放按钮和提示,歌词显示
    connect(mediaPlayer, QMediaPlayer::stateChanged, [=]()
    {
        switch(mediaPlayer->state())
        {
            case QMediaPlayer::PlayingState:
                ui->pushButton_switch->setToolTip("暂停");
                ui->pushButton_switch->setIcon(QIcon(QPixmap(":/icon/Player, pause.svg")));
                // 获取、显示歌词
                lrcMap = search->searchForLrc(currentMusic.getId());
                showLrc();
                break;
            case QMediaPlayer::PausedState:
                ui->pushButton_switch->setToolTip("播放");
                ui->pushButton_switch->setIcon(QIcon(QPixmap(":/icon/Player, play.svg")));
                break;
            default:
                ui->pushButton_switch->setToolTip("播放");
                ui->pushButton_switch->setIcon(QIcon(QPixmap(":/icon/Player, play.svg")));
                break;
        }
    });
    // 音量更新
    connect(mediaPlayer, QMediaPlayer::volumeChanged, [=] {

       if (0 == volume) {
            ui->pushButton_volume->setIcon(QIcon(":/icon/noVolume.svg"));
            ui->pushButton_volume->setToolTip("声音");
       }
       else {

           ui->pushButton_volume->setIcon(QIcon(":/icon/volume.svg"));
           ui->pushButton_volume->setToolTip("静音");
       }
    });
    // 底部栏点击显示歌词
    connect(ui->widget_bottombar, MyBottomBar::clicked, this, [=] {
        ui->tabWidget->setCurrentWidget(ui->tab_lrc);
    });

    // 播放模式改变，更新模式图标和提示
    connect(mediaPlaylist, QMediaPlaylist::playbackModeChanged, [=]
    {
        switch (mediaPlaylist->playbackMode()) {
        case QMediaPlaylist::CurrentItemInLoop:
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/repeat-one-line.svg")));
            ui->pushButton_mode->setToolTip("单曲循环");
            break;
        case QMediaPlaylist::Sequential:
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/order-play-fill.svg")));
            ui->pushButton_mode->setToolTip("顺序播放");
            break;
        case QMediaPlaylist::Loop:
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/repeat.svg")));
            ui->pushButton_mode->setToolTip("列表循环");
            break;
        case QMediaPlaylist::Random:
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/random.svg")));
            ui->pushButton_mode->setToolTip("随机播放");
            break;
        case QMediaPlaylist::CurrentItemOnce:
            break;
        }
    });
    // 搜索框获得焦点
    connect(ui->lineEdit_search, MyLineEdit::focusIn, [=] {
        ui->pushButton_search->setStyleSheet("color:blue;background-color:#edf2ff;");
        ui->lineEdit_search->setStyleSheet("background-color:#edf2ff;");
    });
    // 搜索框失去焦点
    connect(ui->lineEdit_search, MyLineEdit::focusOut, [=] {
        ui->pushButton_search->setStyleSheet("background-color:#f2f2f4;");
        ui->lineEdit_search->setStyleSheet("color:black;background-color:#f2f2f4;");
    });
    // 改变点击项的样式
    connect(mediaPlaylist, QMediaPlaylist::currentMediaChanged, [=] {
        QListWidgetItem * qItem = ui->listWidget_searchResult->item(mediaPlaylist->currentIndex());
        CustomItem *cItem = dynamic_cast<CustomItem *>(qItem);
        changeCurrentPlayingItem(cItem);
    });
}

// 鼠标按下
void HaoMusic::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && event->y() < (ui->widget_header->height() + ui->widget_header->y()))// 判断是否是鼠标左键按下
    {
        mousePress = true;
    }
    // 鼠标在窗口中的坐标
    movePoint = event->globalPos() - pos();// 鼠标的全局坐标-窗口的坐标
    QWidget::mousePressEvent(event);
}

// 鼠标移动
void HaoMusic::mouseMoveEvent(QMouseEvent *event)
{ 
    if(mousePress)// 若是鼠标左键按下则移动窗口
    {
        if (this->isMaximized()) {// 窗口最大化则恢复正常大小
            on_pushButton_maxsize_clicked();
    //        this->showNormal();
    //        ui->pushButton_maxsize->setIcon(QIcon(QPixmap(":/icon/max.svg")));
        }
        QPoint movePos = event->globalPos();// 鼠标现在位置
        this->move(movePos - movePoint);// 窗口应当移动到的坐标=鼠标当前全局坐标-鼠标初始相对窗口坐标
    }
    QWidget::mouseMoveEvent(event);
}

// 鼠标释放
void HaoMusic::mouseReleaseEvent(QMouseEvent *event)
{
    mousePress = false;
    QWidget::mouseReleaseEvent(event);
}

// 鼠标双击
void HaoMusic::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 判断是否左键按下，是否按在标题栏
    if(event->button() == Qt::LeftButton && event->y() < (ui->pushButton_close->height() + ui->pushButton_close->y())) {
        this->on_pushButton_maxsize_clicked();
    }
    QWidget::mouseDoubleClickEvent(event);
}

// 绘图事件
void HaoMusic::paintEvent(QPaintEvent *event)
{
    // 阴影在最大化的情况下不显示
    if (!this->isMaximized()) {
        this->layout()->setContentsMargins(11, 11, 11, 11);
        ui->inner_widget->graphicsEffect()->setEnabled(true);
    }else {
        this->layout()->setContentsMargins(0, 0, 0, 0);
        ui->inner_widget->graphicsEffect()->setEnabled(false);
    }
    // 更新布局
    updateLayout();
    QWidget::paintEvent(event);
}

// 绘制圆角阴影窗体边框
void HaoMusic::paintShadowRadiusWidget()
{
    this->layout()->setContentsMargins(11, 11, 11, 11);
    // 阴影
    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
    //内层窗口添加对应的阴影效果
    shadow_effect->setOffset(0, 0);
    shadow_effect->setColor(QColor(150,150,150));
    shadow_effect->setBlurRadius(6);
    ui->inner_widget->setGraphicsEffect(shadow_effect);
}

// 更新布局
void HaoMusic::updateLayout()
{
    // 设置title大小
    ui->label_title->setFixedWidth(ui->widget_sidebar->width());
    // 设置歌词界面左侧栏布局
    ui->widget_lrc_songInfo->setFixedWidth(width()*0.45);
    ui->label_lrc_albumPic->setFixedSize(0.8*ui->widget_lrc_songInfo->width(),0.8*ui->widget_lrc_songInfo->width());
    ui->label_lrc_songName->setIndent(ui->label_lrc_albumPic->width()*0.12);
    ui->label_lrc_author->setIndent(ui->label_lrc_albumPic->width()*0.12 + 5);
    //设置歌词鼠标左键拖动
    QScroller::grabGesture(ui->listWidget_lrc,QScroller::LeftMouseButtonGesture);
    ui->listWidget_lrc->verticalScrollBar()->setSingleStep(20);
    ui->listWidget_searchResult->verticalScrollBar()->setSingleStep(1);
}

// 关闭窗口
void HaoMusic::on_pushButton_close_clicked()
{
    this->close();
}

// 最小化
void HaoMusic::on_pushButton_minsize_clicked()
{
    this->showMinimized();
}

// 最大化
void HaoMusic::on_pushButton_maxsize_clicked()
{
    if (this->isMaximized()) {// 恢复正常大小
        this->showNormal();
        ui->pushButton_maxsize->setIcon(QIcon(QPixmap(":/icon/max.svg")));
    }
    else {// 最大化
        this->showMaximized();
        ui->pushButton_maxsize->setIcon(QIcon(QPixmap(":/icon/Max2normal.svg")));
    }
}

// 显示加载动画
void HaoMusic::showLoadingPage()
{
    ui->tabWidget->setCurrentWidget(ui->tab_homePage);
    ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_loading);
    loadingMovie->start();
}

// 隐藏歌词
void HaoMusic::on_pushButton_dropDown_clicked()
{
    ui->tabWidget->setCurrentWidget(ui->tab_homePage);
}

// 显示歌词
void HaoMusic::showLrc()
{
    // 清空歌词
    ui->listWidget_lrc->clear();
    lrcKeys.clear();
    currentLrcItem = nullptr;
    // 歌词前空行
    QListWidgetItem *nullItem = new QListWidgetItem("");
    nullItem->setSizeHint(QSize(0, height()*0.5));
    nullItem->setFlags(Qt::NoItemFlags);
    ui->listWidget_lrc->addItem(nullItem);
    // 歌名
    QListWidgetItem *songNameItem = new QListWidgetItem(currentMusic.getSongName());
    songNameItem->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    songNameItem->setData(Qt::StatusTipRole, 0);
    ui->listWidget_lrc->addItem(songNameItem);
    // 歌词
    lrcKeys = lrcMap.keys();
    QMap<int, QString>::iterator iter = lrcMap.begin();
    for (;iter != lrcMap.end(); iter++) {
        QListWidgetItem *item = new QListWidgetItem(iter.value());
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item->setData(Qt::StatusTipRole, iter.key());
        ui->listWidget_lrc->addItem(item);
    }
    // 歌词后空行
    nullItem = new QListWidgetItem("");
    nullItem->setSizeHint(QSize(0, height()*0.5));
    nullItem->setFlags(Qt::NoItemFlags);
    ui->listWidget_lrc->addItem(nullItem);
    // 滚动到歌名所在行
    ui->listWidget_lrc->setCurrentItem(songNameItem);
    ui->listWidget_lrc->scrollToItem(songNameItem);
}

// 歌词滚动
void HaoMusic::lrcRoll(int position)
{
    static int row = 0;
    if (lrcKeys.isEmpty()) {     // 歌词为空，直接返回
        return;
    }
    if (position == 0) {
        row = 0;
    }
    int key = 0;
    int lastKey = 0;
    int nextKey = 0;

    while (row >= 0 && row < lrcKeys.size() - 1) {
        key = lrcKeys.at(row);
        if (row != 0) {
            lastKey = lrcKeys.at(row - 1);
        }
        nextKey = lrcKeys.at(row + 1);
        if (key <= position && nextKey <= position)
            row++;
        else if (key > position && lastKey > position)
            row--;
        else
            break;
    }
    if (row == currentLrcRow) {
        return;
    }
    currentLrcRow = row;
    currentLrcItem = ui->listWidget_lrc->item(currentLrcRow + 2);
    ui->listWidget_lrc->setCurrentItem(currentLrcItem);
    // 滚动到当前歌词
    ui->listWidget_lrc->scrollToItem(currentLrcItem, QAbstractItemView::PositionAtCenter);
    return;
}

// 点击歌词改变歌曲进度
void HaoMusic::on_listWidget_lrc_itemClicked(QListWidgetItem *item)
{
    if (item->data(Qt::StatusTipRole).isNull())
        return;
    int position = item->data(Qt::StatusTipRole).toInt();
    mediaPlayer->setPosition(position);
    if (QMediaPlayer::PausedState == mediaPlayer->state()) {
        mediaPlayer->play();
    }
    if (QMediaPlayer::StoppedState == mediaPlayer->state()) {
        mediaPlayer->setMedia(QUrl(currentMusic.getSongUrl()));
        mediaPlayer->play();
    }
}

// 改变当前播放项
void HaoMusic::changeCurrentPlayingItem(CustomItem *item)
{
    if (currentPlayingItem == item) {
        return;
    }
    if (currentPlayingItem == nullptr) {
        currentPlayingItem = item;
    }
    // 原来的播放项的字体颜色为黑色
    currentPlayingItem->changeFontColor("black");
    // 设置当前播放项的颜色为蓝色
    currentPlayingItem = item;
    currentPlayingItem->changeFontColor("blue");
}

// 给listwidget添加行
void HaoMusic::addItemToListWidget(Music music)
{
    QListWidgetItem* qItem = new QListWidgetItem(ui->listWidget_searchResult);
    ui->listWidget_searchResult->addItem(qItem);
    qItem->setSizeHint(QSize(0, 100));

    CustomItem *myItem = new CustomItem(music, ui->listWidget_searchResult);
    // 绑定右键菜单的信号和槽
    connect(myItem, CustomItem::musicPlay, this, HaoMusic::menuPlayMusicClicked);
    connect(myItem, CustomItem::musicPlay, this, HaoMusic::menuAddToMyFavoriteClicked);
    connect(myItem, CustomItem::musicPlay, this, HaoMusic::menuAddToSonglistClicked);
    ui->listWidget_searchResult->setItemWidget(qItem, myItem);
    // 给QListWidgetItem添加数据
    qItem->setData(Qt::StatusTipRole, music.getId());
}

// 进度变化事件处理函数
void HaoMusic::onPositionChanged(qint64 position)
{
    ui->horizontalSlider_musicProgress->setValue(position);//设置滑块位置
    int currentPos = ui->horizontalSlider_musicProgress->value();
    // 设置播放进度提示
    QTime displayTime(0, (currentPos/60000) % 60,(currentPos/1000) % 60);
    ui->horizontalSlider_musicProgress->setToolTip(displayTime.toString("mm:ss"));
    // 歌词滚动
    lrcRoll(position);
}

// 时长变化处理函数
void HaoMusic::onDurationChanged(qint64 duration)
{
    updateBottomMusicInfo();
}

// 上一首
void HaoMusic::on_pushButton_lastsong_clicked()
{
    mediaPlaylist->setCurrentIndex(mediaPlaylist->previousIndex());
    mediaPlayer->setPlaylist(mediaPlaylist);
    mediaPlayer->play();
}

// 播放按钮
void HaoMusic::on_pushButton_switch_clicked()
{
    if (QMediaPlayer::PlayingState == mediaPlayer->state()) {
        mediaPlayer->pause();
    }
    else {
        mediaPlayer->play();
    }
}

// 播放当前项
void HaoMusic::playingTheItem(CustomItem *item)
{
    if (item == nullptr) {
        return;
    }
    changeCurrentPlayingItem(item);
    // 获取当前点击的音乐
    currentMusic = item->getMusic();
    // 通过音乐id获取url
    int id = currentMusic.getId();
    QString songUrl = search->searchForSongUrl(id);
    if (songUrl.isEmpty()) {
        qDebug()<<__FILE__<<__LINE__<<"songUrl is empty";
        return;
    }
    // 音乐播放
    mediaPlayer->setMedia(QUrl(songUrl));
    mediaPlayer->play();
}

// 下一首
void HaoMusic::on_pushButton_nextsong_clicked()
{
    mediaPlaylist->setCurrentIndex(mediaPlaylist->nextIndex());
    mediaPlayer->setPlaylist(mediaPlaylist);
    mediaPlayer->play();
}

// 播放模式
void HaoMusic::on_pushButton_mode_clicked()
{
    switch (mediaPlaylist->playbackMode()) {
        //列表循环->顺序播放
        case QMediaPlaylist::Loop:
            mediaPlaylist->setPlaybackMode(QMediaPlaylist::Sequential);
            break;
        //顺序播放->随机播放
        case QMediaPlaylist::Sequential:
            mediaPlaylist->setPlaybackMode(QMediaPlaylist::Random);
            break;

        //随机播放->单曲循环
        case QMediaPlaylist::Random:
            mediaPlaylist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
            break;

        //单曲循环->列表循环
        case QMediaPlaylist::CurrentItemInLoop:
            mediaPlaylist->setPlaybackMode(QMediaPlaylist::Loop);
            break;
        case QMediaPlaylist::CurrentItemOnce:
            break;
    }
}

// 声音
void HaoMusic::on_pushButton_volume_clicked()
{
    if (0 == mediaPlayer->volume()) {
        mediaPlayer->setVolume(volume);
        ui->horizontalSlider_volume->setEnabled(true);
        ui->horizontalSlider_volume->setValue(volume);
        ui->pushButton_volume->setIcon(QIcon(":/icon/volume.svg"));
        ui->pushButton_volume->setToolTip("静音");
    }
    else {
        mediaPlayer->setVolume(0);
        ui->horizontalSlider_volume->setEnabled(false);
        ui->pushButton_volume->setIcon(QIcon(":/icon/noVolume.svg"));
        ui->pushButton_volume->setToolTip("声音");
    }

}

// 音量条拖动事件处理函数
void HaoMusic::on_horizontalSlider_volume_valueChanged(int value)
{
    volume = value;
    mediaPlayer->setVolume(volume);
}

// 音乐进度条点击
void HaoMusic::sliderClicked()
{
    mediaPlayer->setPosition(ui->horizontalSlider_musicProgress->value());
}

// 搜索
void HaoMusic::on_pushButton_search_clicked()
{

    if (searchKeywords == ui->lineEdit_search->text()) {  // 禁止重复搜索
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_searchResult);
        return;
    }
    searchKeywords = ui->lineEdit_search->text(); // 获取搜索关键词
    if (searchKeywords.isEmpty()) {   // 判断关键词是否为空
        return;
    }
    ui->lineEdit_search->clearFocus();      // 清除搜索框焦点
    ui->listWidget_searchResult->clear();   // 清空搜索结果列表
    // 进入加载动画页面
    showLoadingPage();
    // 切换到搜索页面,暂停加载动画
    QTimer::singleShot(loadingTimes, [=] {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_searchResult);
        loadingMovie->stop();
    });
    // label_searchKeywords显示搜索关键词
    ui->label_searchKeywords->setText(QString("“%1”").arg(searchKeywords));
    // 搜索关键词，返回搜索结果列表
    searchResultMusicList = search->search(searchKeywords);
    showSearchResult();

}

// 显示搜索结果
void HaoMusic::showSearchResult()
{
    for (int i = 0; i < searchResultMusicList.size(); i++) {
        addItemToListWidget(searchResultMusicList.at(i));
    }
}

// 搜索结果列表双击播放
void HaoMusic::on_listWidget_searchResult_itemDoubleClicked(QListWidgetItem *item)
{
    // 获取当前行的customitem
    CustomItem *cItem = dynamic_cast<CustomItem *>(ui->listWidget_searchResult->itemWidget(item));
    playingTheItem(cItem);
}

// 更新播放列表
void HaoMusic::updateMediaPlaylist(QList<Music> musicList)
{
    mediaPlaylist->clear();
    for (int i = 0; i < musicList.size(); i++) {
        mediaPlaylist->addMedia(QUrl(musicList.at(i).getSongUrl()));
    }
}

// 更新当前播放歌曲信息
void HaoMusic::updateBottomMusicInfo()
{
    // 获取专辑时长，初始化进度条
    int duration = currentMusic.songDuration();
    ui->horizontalSlider_musicProgress->setRange(0, duration);//根据播放时长来设置滑块的范围
    ui->horizontalSlider_musicProgress->setEnabled(duration>0);
    ui->horizontalSlider_musicProgress->setSingleStep(100);
    ui->horizontalSlider_musicProgress->setPageStep(duration/20);//以及每一步的步数
    // 设置歌曲信息
    ui->label_author->setText(currentMusic.getAuthor());
    ui->label_songname->setText(currentMusic.getSongName());
    ui->label_lrc_author->setText(currentMusic.getAuthor());
    ui->label_lrc_songName->setText(currentMusic.getSongName());

    QPixmap pixmap = currentMusic.albumPic();
    // 如果图片为空,重新加载
    if (pixmap.isNull()) {
        pixmap = MyHttp::showAlbumPic(currentMusic.albumPicUrl());
    }
    ui->label_albumPic->setPixmap(pixmap);
    ui->label_lrc_albumPic->setPixmap(pixmap);
//    ui->label_albumPic->setPixmap(pixmap.scaled(ui->label_albumPic->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
//    ui->label_lrc_albumPic->setPixmap(pixmap.scaled(ui->label_lrc_albumPic->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

// 打开播放历史页面
void HaoMusic::on_pushButton_recentlyplayed_clicked()
{
    ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_recentlyPlayed);
}

// 打开默认歌单页面
void HaoMusic::on_pushButton_defaultSongList_clicked()
{
    showLoadingPage();
    QTimer::singleShot(loadingTimes, [=] {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_defalutSongList);
        loadingMovie->stop();
    });
}

// 打开本地音乐页面
void HaoMusic::on_pushButton_localmusic_clicked()
{
    showLoadingPage();
    QTimer::singleShot(loadingTimes, [=] {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_localmusic);
        loadingMovie->stop();
    });

}

// 打开我喜欢的音乐界面
void HaoMusic::on_pushButton_favorite_clicked()
{
    showLoadingPage();
    QTimer::singleShot(loadingTimes, [=] {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_favorite);
        loadingMovie->stop();
    });
}

// 右键菜单点击播放音乐
void HaoMusic::menuPlayMusicClicked(CustomItem *item)
{
    playingTheItem(item);
}

// 添加到我喜欢的音乐
void HaoMusic::menuAddToMyFavoriteClicked(CustomItem *item)
{
    favoriteMusicList.push_back(item->getMusic());
}

// 添加到我的歌单
void HaoMusic::menuAddToSonglistClicked(CustomItem *item)
{

}


#include "haomusic.h"
#include "ui_haomusic.h"

#include "mybottombar.h"
#include "mylineedit.h"

#include <QButtonGroup>
#include <QFocusEvent>
#include <QListWidgetItem>
#include <QMenu>
#include <QMovie>
#include <QPainter>
#include <QScroller>
#include <QScrollBar>
#include <QThreadPool>
#include <QTimer>

HaoMusic::HaoMusic(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HaoMusic)
{
    ui->setupUi(this);
    // 设置托盘图标
    setTrayIcon();
    // 初始化播放器
    initPlayer();
    // 连接信号和槽
    connectSignalsAndSlots();
    // 初始化音乐列表
    initMusicList();
}

HaoMusic::~HaoMusic()
{
    delete ui;
    m_db.close();
}

// 初始化
void HaoMusic::initPlayer()
{
    paintShadowRadiusWidget();
    // 网络处理类初始化
    search = new MyHttp(this);
    // 播放器初始化
    mediaPlayer = new QMediaPlayer(this);
    mediaPlaylist = new MyMediaPlaylist(this);
    mediaPlaylist->setPlaybackMode(MyMediaPlaylist::Loop);//设置播放模式(顺序播放，单曲循环，随机播放等)
    mediaPlayer->setPlaylist(mediaPlaylist);
    mediaPlayer->setAudioRole(QAudio::MusicRole);
    mediaPlayer->setVolume(volume);
    ui->horizontalSlider_volume->setValue(volume);
    // 按钮互斥
    QButtonGroup *btnGroup = new QButtonGroup(this);
    btnGroup->setExclusive(true);
    btnGroup->addButton(ui->pushButton_localmusic);
    btnGroup->addButton(ui->pushButton_favorite);
    btnGroup->addButton(ui->pushButton_recentlyplayed);
    btnGroup->addButton(ui->pushButton_defaultSongList);
    btnGroup->addButton(ui->pushButton_search);
    connect(btnGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked), this, [=](QAbstractButton *btn) {
        QList<QAbstractButton*> buttons = btnGroup->buttons();
        foreach (QAbstractButton *button, buttons) {
            button->setEnabled(true);
        }
        btn->setEnabled(false);
        ui->pushButton_search->setEnabled(true);
    });
    // 设置列表属性
    ui->listWidget_favorite->setListType(MyListWidget::FAVORITE);
    ui->listWidget_localMusic->setListType(MyListWidget::LOCAL);
    ui->listWidget_searchResult->setListType(MyListWidget::SEARCHRESULT);
    ui->listWidget_history->setListType(MyListWidget::HISTORY);
    // 加载动画
    loadingMovie = new QMovie(":/icon/loading.gif");
    ui->label_loading->setMovie(loadingMovie);
    //设置歌词鼠标左键拖动
    QScroller::grabGesture(ui->listWidget_lrc,QScroller::LeftMouseButtonGesture);
    ui->listWidget_lrc->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listWidget_lrc->verticalScrollBar()->setSingleStep(10);
}

// 初始化音乐列表
void HaoMusic::initMusicList()
{
    // 连接数据库
    m_db.open();
    historyMusicList = m_db.query(MyListWidget::HISTORY);
    favoriteMusicList = m_db.query(MyListWidget::FAVORITE);
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
    // 绑定播放进度改变事件
    connect(mediaPlayer,&QMediaPlayer::positionChanged,this,&HaoMusic::onPositionChanged);
    // 按Enter键搜索
    connect(ui->lineEdit_search, &QLineEdit::returnPressed, [=] {
        ui->pushButton_search->click();
        ui->lineEdit_search->focusOut();
    });
    // 播放状态改变，更新播放按钮和提示
    connect(mediaPlayer, QMediaPlayer::stateChanged, [=]()
    {
        switch(mediaPlayer->state())
        {
            case QMediaPlayer::PlayingState:
                ui->pushButton_switch->setToolTip("暂停");
                ui->pushButton_switch->setIcon(QIcon(QPixmap(":/icon/Player, pause.svg")));

                break;
            default:
                ui->pushButton_switch->setToolTip("播放");
                ui->pushButton_switch->setIcon(QIcon(QPixmap(":/icon/Player, play.svg")));
                break;
        }
    });
    // 当前媒体状体改变
    connect(mediaPlayer, QMediaPlayer::mediaStatusChanged, [=]()
    {
        if (QMediaPlayer::EndOfMedia == mediaPlayer->mediaStatus()) {
            mediaPlaylist->next();
        }
    });
    // 播放歌曲改变
    connect(mediaPlayer, QMediaPlayer::currentMediaChanged, this, [=] {
        int index = mediaPlaylist->currentIndex();
        qDebug()<<"index:"<<index;
        if(0 > index) {
            return;
        }
        currentMusic = musicList.at(index);
        // 存入播放历史列表
        addToHistoryList(currentMusic);
        qDebug() << __FILE__ << __LINE__ << "the current playing music is:" << currentMusic.getSongName();
        updateBottomMusicInfo();
        // 获取、显示歌词
        lrcMap = search->searchForLrc(currentMusic.getId());
        showLrc();
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
        showLrcPage();
    });
    // 搜索框获得焦点
    connect(ui->lineEdit_search, MyLineEdit::focusIn, [=] {
        ui->pushButton_search->setStyleSheet("background-color:#edf2ff;");
        ui->lineEdit_search->setStyleSheet("color:blue;background-color:#edf2ff;");
    });
    // 搜索框失去焦点
    connect(ui->lineEdit_search, MyLineEdit::focusOut, [=] {
        ui->pushButton_search->setStyleSheet("background-color:#f2f2f4;");
        ui->lineEdit_search->setStyleSheet("color:black;background-color:#f2f2f4;");
    });
    // 绑定列表菜单点击事件和列表项双击事件
    QList<MyListWidget *> listwidgets = this->findChildren<MyListWidget *>();
    foreach (MyListWidget *listwidget, listwidgets) {
        connect(listwidget, &MyListWidget::menuClicked, this, &HaoMusic::onMenuClicked);
        connect(listwidget, &MyListWidget::customItemDoubleClicked, this, &HaoMusic::onCustomItemDoubleClicked);

        // 设置列表滚动方式和单步步长
        listwidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        listwidget->verticalScrollBar()->setSingleStep(10);
    }
}

// 鼠标按下
void HaoMusic::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
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
    // 边框阴影在最大化的情况下不显示
    if (!this->isMaximized()) {
        this->layout()->setContentsMargins(11, 11, 11, 11);
        ui->inner_widget->graphicsEffect()->setEnabled(true);
    }else {
        this->layout()->setContentsMargins(0, 0, 0, 0);
        ui->inner_widget->graphicsEffect()->setEnabled(false);
    }
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

// 关闭窗口
void HaoMusic::on_pushButton_close_clicked()
{
    QThreadPool::globalInstance()->waitForDone();
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
    songNameItem->setFlags(Qt::NoItemFlags);
    songNameItem->setForeground(Qt::black);
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
}

// 歌词滚动
void HaoMusic::lrcRoll(int position)
{
    // 当前歌词所在行
    static int row = 0;
    // 歌词为空，直接返回
    if (lrcKeys.isEmpty()) {
        return;
    }
    // 歌曲切换，行数清零
    if (position == 0) {
        row = 0;
    }
    // 当前歌词进度
    int key = 0;
    // 上一行歌词进度
    int lastKey = 0;
    // 下一行歌词进度
    int nextKey = 0;
    // 找到当前歌曲进度所对应歌词
    while (row >= 0 && row < lrcKeys.size() - 1) {
        key = lrcKeys.at(row);
        if (row != 0) {
            lastKey = lrcKeys.at(row - 1);
        }
        nextKey = lrcKeys.at(row + 1);
        // 歌曲进度大于当前行和下一行歌词进度，行数+1，反之则-1
        if (key <= position && nextKey <= position)
            row++;
        else if (key >= position && lastKey >= position) {
            // 行数为0时不做处理
            if (row == 0)
                break;
            row--;
        }
        else
            break;
    }
    // 歌词所在行无变化则返回
    if (row == currentLrcRow) {
        return;
    }
    // 更新当前歌词所在行
    currentLrcRow = row;
    // 更新歌词所在行对应的item，并选中
    currentLrcItem = ui->listWidget_lrc->item(currentLrcRow + 2);
    ui->listWidget_lrc->setCurrentItem(currentLrcItem);
    // 滚动到当前歌词
    ui->listWidget_lrc->scrollToItem(currentLrcItem, QAbstractItemView::PositionAtCenter);
}

// 点击歌词更新进度
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
        mediaPlaylist->setCurrentIndex(mediaPlaylist->currentIndex());
//        mediaPlayer->play();
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
    item->changeFontColor("blue");
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

// 上一首
void HaoMusic::on_pushButton_lastsong_clicked()
{
    mediaPlaylist->setCurrentIndex(mediaPlaylist->previousIndex());
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
    mediaPlaylist->setCurrentIndex(musicList.indexOf(currentMusic));
    // 音乐播放
    mediaPlayer->play();
}

// 下一首
void HaoMusic::on_pushButton_nextsong_clicked()
{
    mediaPlaylist->setCurrentIndex(mediaPlaylist->nextIndex());
    mediaPlayer->play();
}

// 播放模式
void HaoMusic::on_pushButton_mode_clicked()
{
    switch (mediaPlaylist->playbackMode()) {
        //列表循环->顺序播放
        case MyMediaPlaylist::Loop:
            mediaPlaylist->setPlaybackMode(MyMediaPlaylist::Sequential);
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/order-play-fill.svg")));
            ui->pushButton_mode->setToolTip("顺序播放");
            break;
        //顺序播放->随机播放
        case MyMediaPlaylist::Sequential:
            mediaPlaylist->setPlaybackMode(MyMediaPlaylist::Random);
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/random.svg")));
            ui->pushButton_mode->setToolTip("随机播放");
            break;

        //随机播放->单曲循环
        case MyMediaPlaylist::Random:
            mediaPlaylist->setPlaybackMode(MyMediaPlaylist::CurrentItemInLoop);
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/repeat-one-line.svg")));
            ui->pushButton_mode->setToolTip("单曲循环");
            break;

        //单曲循环->列表循环
        case MyMediaPlaylist::CurrentItemInLoop:
            mediaPlaylist->setPlaybackMode(MyMediaPlaylist::Loop);
            ui->pushButton_mode->setIcon(QIcon(QPixmap(":/icon/repeat.svg")));
            ui->pushButton_mode->setToolTip("列表循环");
            break;
        case MyMediaPlaylist::CurrentItemOnce:
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

// 获取编辑框输入
void HaoMusic::on_lineEdit_search_textChanged(const QString &arg1)
{
    //    searchKeywords = arg1;
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
    // 当前播放项置为空
    if (listType == MyListWidget::SEARCHRESULT)
        currentPlayingItem = nullptr;
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
    searchResultMusicList.clear();
    searchResultMusicList = search->search(searchKeywords);
    showSearchResult();
}

// 显示搜索结果
void HaoMusic::showSearchResult()
{
    // 搜索列表已更新
    isSearchResultUpdate = true;
    // 更新搜索结果列表
    ui->listWidget_searchResult->setMusicList(searchResultMusicList);
}

// 双击列表项播放
void HaoMusic::onCustomItemDoubleClicked(CustomItem *item)
{
    // 如果要播放的音乐所在列表与当前音乐所在列表不同，需要更新音乐播放列表
    if (listType != item->getItemType()) {
        listType = item->getItemType();
        updateMediaPlaylist();
    }
    // 如果还是搜索结果列表，判断搜索结果列表是否更新，如果已更新则更新音乐播放列表
    else if(listType == MyListWidget::SEARCHRESULT) {
        if (isSearchResultUpdate) {
            updateMediaPlaylist();
            isSearchResultUpdate = false;
        }
    }
    // 播放音乐
    playingTheItem(item);
}

// 更新播放列表
void HaoMusic::updateMediaPlaylist()
{
    // 切换播放列表
    switch (listType) {
    case MyListWidget::SEARCHRESULT:
        musicList = searchResultMusicList;
        break;
    case MyListWidget::FAVORITE:
        musicList = favoriteMusicList;
        break;
    case MyListWidget::SONGLIST:

        break;
    case MyListWidget::LOCAL:
        musicList = localMusicList;
        break;
    case MyListWidget::HISTORY:
        musicList = historyMusicList;
    default:
        break;
    }
    // 清空音乐播放列表
    mediaPlaylist->clear();
    // 初始化音乐播放列表
    for (int i = 0; i < musicList.size(); i++) {
        int id = musicList.at(i).getId();
        mediaPlaylist->addMedia(id);
    }
}

// 更新当前播放歌曲信息
void HaoMusic::updateBottomMusicInfo()
{
    // 获取专辑时长，初始化进度条
    int duration = currentMusic.songDuration();
    ui->horizontalSlider_musicProgress->setRange(0, duration);//根据播放时长来设置滑块的范围
    ui->horizontalSlider_musicProgress->setEnabled(duration>0);
    ui->horizontalSlider_musicProgress->setSingleStep(10);
    ui->horizontalSlider_musicProgress->setPageStep(duration/20);//以及每一步的步数
    // 设置歌曲信息
    ui->label_author->setText(currentMusic.getAuthor());
    ui->label_songname->setText(currentMusic.getSongName());
    ui->label_lrc_author->setText(currentMusic.getAuthor());
    ui->label_lrc_songName->setText(currentMusic.getSongName());

//    QPixmap img = currentPlayingItem->getAlbumPic();
    ui->label_albumPic->setRadiusPixmap(currentMusic.albumPicUrl());
//        ui->label_albumPic->makeRadiusPixmap(img);

    ui->label_lrc_albumPic->setRadiusPixmap(currentMusic.albumPicUrl());
//    ui->label_lrc_albumPic->setRadiusPixmap(currentMusic.albumPicUrl());
    ui->tab_lrc->setGaussblurBackground(currentMusic.albumPicUrl());
//    ui->inner_widget->setGaussblurBackground(currentMusic.albumPicUrl());
}

// 打开播放历史页面
void HaoMusic::on_pushButton_recentlyplayed_clicked()
{
    if (isHistoryMusicListShow) {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_recentlyPlayed);
        return;
    }
    // 显示加载动画
    showLoadingPage();
    QTimer::singleShot(loadingTimes, [=] {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_recentlyPlayed);
        loadingMovie->stop();
    });
    // 我喜欢的音乐列表为空则返回
    if (historyMusicList.isEmpty()) {
        return;
    }
    isHistoryMusicListShow = true;
    // 显示喜欢的音乐
    ui->listWidget_history->setMusicList(historyMusicList);
}

// 添加到播放历史列表
void HaoMusic::addToHistoryList(Music &music)
{
    int index = historyMusicList.indexOf(music);
    // 判断歌曲是否存在
    if (-1 == index) {  // 不存在则添加到顶部
        if (historyMusicList.size() > 50)  {  // 维持播放历史列表大小在50
            historyMusicList.pop_back();    // 删除历史播放列表最后一项
            m_db.remove(music.getId(), MyListWidget::HISTORY);
            if (isHistoryMusicListShow) {   // 如果列表已经加载，删除最后一个item
                QListWidgetItem *item = ui->listWidget_history->item(index);
                CustomItem *cItem = qobject_cast<CustomItem *>(ui->listWidget_history->itemWidget(item));
                ui->listWidget_history->takeItem(index);
                delete item;
                delete cItem;
            }
        }
        historyMusicList.push_front(music); // 添加历史播放列表到顶部
        m_db.insert(music, MyListWidget::HISTORY);
        if (isHistoryMusicListShow) {   // 如果列表已经加载，顶部添加item
            ui->listWidget_history->insertCustomItem(music);
        }
    }
    else {  // 存在则移动到顶部
        historyMusicList.move(index, 0);
        m_db.remove(music.getId(), MyListWidget::HISTORY);
        m_db.insert(music, MyListWidget::HISTORY);
        if (isHistoryMusicListShow) {   // 如果列表已经加载，则移动item到顶部
            delete ui->listWidget_history->takeItem(index);
            ui->listWidget_history->insertCustomItem(music);
        }
    }
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
    // 如果已经显示过则直接跳转页面
    if (isFavoriteMusicListShow) {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_favorite);
        return;
    }
    isFavoriteMusicListShow = true;
    // 显示加载动画
    showLoadingPage();
    QTimer::singleShot(loadingTimes, [=] {
        ui->tabWidget_switchcontent->setCurrentWidget(ui->tab_favorite);
        loadingMovie->stop();
    });
    // 我喜欢的音乐列表为空则返回
    if (favoriteMusicList.isEmpty()) {
        return;
    }
    // 显示喜欢的音乐
    ui->listWidget_favorite->setMusicList(favoriteMusicList);
}

// 菜单点击处理事件
void HaoMusic::onMenuClicked(CustomItem *item, int itemType)
{
    switch (itemType) {
    case CustomItem::PLAY:
        menuPlayMusicClicked(item);
        break;
    case CustomItem::ADDTOFAVORITE:
        menuAddToMyFavoriteClicked(item);
        break;
    case CustomItem::ADDTOSONGLIST:
        menuAddToSonglistClicked(item);
        break;
    case CustomItem::REMOVEFROMFAVORITE:
        menuRemoveFromSongList(item);
        break;
    case CustomItem::REMOVEFROMSONGLIST:

        break;
    default:
        break;
    }
}

// 右键菜单点击播放音乐
void HaoMusic::menuPlayMusicClicked(CustomItem *item)
{
    onCustomItemDoubleClicked(item);
}

// 添加到我喜欢的音乐
void HaoMusic::menuAddToMyFavoriteClicked(CustomItem *item)
{
    Music music = item->getMusic();
    if (-1 != favoriteMusicList.indexOf(music)) {
        qDebug()<<"歌曲已存在";
        return;
    }
    favoriteMusicList.push_front(music);
    m_db.insert(music);
    // 如果我喜欢的音乐页面已经加载，直接插入item
    if (isFavoriteMusicListShow)
        ui->listWidget_favorite->insertCustomItem(music, 0);
    // 如果正在播放的是我喜欢的音乐，需要更新播放列表
    if (listType == MyListWidget::FAVORITE) {
        musicList.push_front(music);
        int index = mediaPlaylist->currentIndex();
        mediaPlaylist->insertMedia(0, music.getId());
        mediaPlaylist->setCurrentIndex(index+1);
    }
}

// 添加到我的歌单
void HaoMusic::menuAddToSonglistClicked(CustomItem *item)
{

}

// 从歌单中移除
void HaoMusic::menuRemoveFromSongList(CustomItem *item)
{
    // 找到正在播放歌曲所在歌单，并移除该歌曲
    QList<MyListWidget *> lists = this->findChildren<MyListWidget *>();
    foreach (MyListWidget* list, lists) {
        if (list->getListType() == item->getItemType()) {
            list->removeCustomItem(item);
        }
    }
    // 从播放列表、音乐列表移除
    int index = musicList.indexOf(item->getMusic());
    mediaPlaylist->removeMedia(index);
    musicList.takeAt(index);
    // 从数据库移除
    m_db.remove(item->getMusic().getId(), item->getItemType());
}

// 显示歌词界面
void HaoMusic::showLrcPage()
{
    ui->tabWidget->setCurrentWidget(ui->tab_lrc);
    ui->listWidget_lrc->setCurrentItem(currentLrcItem);
    ui->listWidget_lrc->scrollToItem(currentLrcItem, QAbstractItemView::PositionAtCenter);
}

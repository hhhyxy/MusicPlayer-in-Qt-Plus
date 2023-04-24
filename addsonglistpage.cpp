#include "addsonglistpage.h"
#include "ui_addsonglistpage.h"
#include <QDebug>
#include <QMouseEvent>
AddSongListPage::AddSongListPage(QWidget *parent) :
    ShadowWidget(parent),
    ui(new Ui::AddSongListPage)
{
    ui->setupUi(this);
    this->resize(600, 600);
    ui->lineEdit_listName->hide();  // 隐藏编辑框
    m_db = MusicDB::instance();     // 获取数据库连接
    // 搜索框失去焦点
    connect(ui->lineEdit_listName, &MyLineEdit::focusOut, [=] {
        ui->lineEdit_listName->hide();
        ui->pushButton_newSongList->show();
    });
    // 按确认键创建歌单
    connect(ui->lineEdit_listName, &MyLineEdit::returnPressed, this, &AddSongListPage::createSonglist);
}

AddSongListPage::~AddSongListPage()
{
    delete ui;
}

void AddSongListPage::init(Music& music)
{
    this->setFocus();
    // 移动到正中央
    this->move((parentWidget()->width() - this->width()) / 2, (parentWidget()->height() - this->height()) / 2);
    // 从数据库获取所有歌单
    QMap<int, QString> songlists = m_db->queryList();
    // 初始化歌单列表
    ui->listWidget_songlistWidget->clear();
    this->addItems(songlists);
    this->music = music;
    this->show();
}

// 关闭按钮
void AddSongListPage::on_pushButton_close_clicked()
{
    this->close();
}

// 往列表添加歌单
void AddSongListPage::addItems(QMap<int, QString>& lists)
{
    QMapIterator<int, QString> iter(lists);
    while (iter.hasNext()) {
        iter.next();
        addItem(iter.key(), iter.value());
    }
}

// 向listwidget添加歌单信息
void AddSongListPage::addItem(int id, QString name)
{
    QListWidgetItem *item = new QListWidgetItem(name);
    item->setTextAlignment(Qt::AlignCenter);
    item->setData(Qt::UserRole, id);
    ui->listWidget_songlistWidget->addItem(item);
}

// 新建歌单
void AddSongListPage::on_pushButton_newSongList_clicked()
{
    ui->pushButton_newSongList->hide();
    ui->lineEdit_listName->clear();
    ui->lineEdit_listName->show();
    ui->lineEdit_listName->setFocus();
}

// 添加歌曲到歌单
void AddSongListPage::on_listWidget_songlistWidget_itemClicked(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();
    emit add(id, music);
    this->close();
}

// 创建歌单
void AddSongListPage::createSonglist()
{
    QString name = ui->lineEdit_listName->text();
    // 歌单查重
    int cnt = ui->listWidget_songlistWidget->count();
    for (int i = 0; i < cnt; i++) {
        if (name == ui->listWidget_songlistWidget->item(i)->text()) {
            ui->lineEdit_listName->clear();
            qDebug() << __FILE__ << __LINE__ << "songlist is existed!";
            return;
        }
    }
    // 隐藏编辑框，显示按钮
    ui->lineEdit_listName->hide();
    ui->pushButton_newSongList->show();
    // 创建歌单
    int id = m_db->insertList(name);
    addItem(id, name);
    emit create(id, name);
}


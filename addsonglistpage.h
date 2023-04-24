#ifndef ADDSONGLISTPAGE_H
#define ADDSONGLISTPAGE_H

#include "shadowwidget.h"
#include "musicdb.h"
namespace Ui {
class AddSongListPage;
}
/*
 * 添加到歌单的弹出界面
*/
class AddSongListPage : public ShadowWidget
{
    Q_OBJECT

public:
    explicit AddSongListPage(QWidget *parent = nullptr);
    ~AddSongListPage();
    void init(Music& music);

signals:
    // 创建歌单
    void create(int id, QString name);
    // 向歌单中添加歌曲
    void add(int id, Music music);

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_newSongList_clicked();

    void on_listWidget_songlistWidget_itemClicked(QListWidgetItem *item);

    void createSonglist();
private:
    Ui::AddSongListPage *ui;
    MusicDB *m_db = nullptr;  // 数据库连接

    Music music;

private:
    void addItems(QMap<int, QString> &lists);

    void addItem(int id, QString name);
};

#endif // ADDSONGLISTPAGE_H

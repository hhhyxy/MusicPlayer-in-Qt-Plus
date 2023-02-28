#ifndef ADDSONGLISTPAGE_H
#define ADDSONGLISTPAGE_H

#include <QWidget>

namespace Ui {
class AddSongListPage;
}

class AddSongListPage : public QWidget
{
    Q_OBJECT

public:
    explicit AddSongListPage(QWidget *parent = nullptr);
    ~AddSongListPage();

private:
    Ui::AddSongListPage *ui;
};

#endif // ADDSONGLISTPAGE_H

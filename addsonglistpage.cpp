#include "addsonglistpage.h"
#include "ui_addsonglistpage.h"

AddSongListPage::AddSongListPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddSongListPage)
{
    ui->setupUi(this);
}

AddSongListPage::~AddSongListPage()
{
    delete ui;
}

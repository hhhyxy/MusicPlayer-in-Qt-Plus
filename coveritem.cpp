#include "coveritem.h"
#include "ui_coveritem.h"

#include <QMouseEvent>

CoverItem::CoverItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoverItem)
{
    ui->setupUi(this);
    margin = this->layout()->margin();
    this->setMouseTracking(true);
}

CoverItem::~CoverItem()
{
    delete ui;
}

void CoverItem::setCover(QString url)
{
    ui->label_cover->setRadiusPixmap(url);

}

void CoverItem::setCoverSize(QSize size)
{
    ui->label_cover->resize(size);
}

void CoverItem::setName(QString name)
{
    ui->label_name->setEllipsisText(name);
}

void CoverItem::setMargin(int mar)
{
    ui->verticalLayout->setMargin(mar);
    margin = mar;
}

void CoverItem::normal()
{
    qDebug() << "normal";
    ui->label_name->show();
}

void CoverItem::zoomOut()
{
    qDebug() << "zoom out";
    ui->label_name->hide();
}

void CoverItem::mouseMoveEvent(QMouseEvent *event)
{
    if (in)
        return;
    zoomOut();
    in = true;
    QWidget::mouseMoveEvent(event);
}

void CoverItem::leaveEvent(QEvent *event)
{
    if (!in)
        return;
    normal();
    in = false;
    QWidget::leaveEvent(event);
}


#include "ellipsislabel.h"

EllipsisLabel::EllipsisLabel(QWidget *parent)
    : QLabel{parent}
{

}

void EllipsisLabel::setEllipsisText(QString text)
{
    this->setToolTip(text);
    QString ellipsis = this->fontMetrics().elidedText(text, Qt::ElideRight, this->width());
    this->setText(ellipsis);
}

void EllipsisLabel::resizeEvent(QResizeEvent *event)
{
    QString ellipsis = this->fontMetrics().elidedText(this->toolTip(), Qt::ElideRight, this->width());
    this->setText(ellipsis);
}

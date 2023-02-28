#include "mylineedit.h"
#include <QFocusEvent>

MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit{parent}
{

}

void MyLineEdit::focusInEvent(QFocusEvent *e)
{
    emit focusIn();
    QLineEdit::focusInEvent(e);
}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
    emit focusOut();
    QLineEdit::focusOutEvent(e);
}

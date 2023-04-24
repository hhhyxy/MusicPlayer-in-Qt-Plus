#include "mylineedit.h"
#include <QFocusEvent>

MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit{parent}
{

}

void MyLineEdit::focusInEvent(QFocusEvent *e)
{
    emit focusIn();
    setStyleSheet("color:blue;background-color:#edf2ff;");
    QLineEdit::focusInEvent(e);
}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
    emit focusOut();
    setStyleSheet("color:black;background-color:#f2f2f4;");
    QLineEdit::focusOutEvent(e);
}

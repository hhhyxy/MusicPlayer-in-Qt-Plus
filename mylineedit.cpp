#include "mylineedit.h"
#include <QFocusEvent>

MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit{parent}
{
    this->setFocus(Qt::MouseFocusReason);
    list = nullptr;
}

void MyLineEdit::setDropDownList(QListWidget *theList)
{
    list = theList;
}

bool MyLineEdit::event(QEvent *e)
{
    if (list && e->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Tab) {
            this->setText(list->currentIndex().data(Qt::DisplayRole).toString());
            return true;
        }
    }
    return QLineEdit::event(e);
}

void MyLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (list) {
        if (Qt::Key_Up == event->key()) {
            int row = list->currentRow();
            if (row) {
                list->setCurrentRow(list->currentRow() - 1);
            }
            return;
        }
        if (Qt::Key_Down == event->key() ) {
            int row = list->currentRow();
            if (row < list->count() - 1) {
                list->setCurrentRow(list->currentRow() + 1);
            }
            return;
        }
        if (Qt::Key_Return == event->key()) {
            QString word = list->currentIndex().data(Qt::DisplayRole).toString();
            if (!word.isEmpty())
                this->setText(word);
            this->blockSignals(false);
            emit returnPressed();
            return;
        }
    }
    QLineEdit::keyPressEvent(event);
}

void MyLineEdit::focusInEvent(QFocusEvent *e)
{
    emit focusIn();
    setStyleSheet("QLineEdit{color:blue;background-color:#edf2ff;}");
    QLineEdit::focusInEvent(e);
}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
    emit focusOut();
    setStyleSheet("QLineEdit{color:black;background-color:#f2f2f4;}");
    QLineEdit::focusOutEvent(e);
}

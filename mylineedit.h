#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QWidget>
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = nullptr);

signals:
    void focusIn();
    void focusOut();
protected:
    void focusInEvent(QFocusEvent * e);
    void focusOutEvent(QFocusEvent *e);
};

#endif // MYLINEEDIT_H

#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QWidget>
#include <QLineEdit>

/*
 * 主界面顶部输入框
 */
class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = nullptr);

signals:
    // 获得焦点信号
    void focusIn();
    // 失去焦点信号
    void focusOut();
protected:
    // 获得焦点事件
    void focusInEvent(QFocusEvent * e);
    // 失去焦点事件
    void focusOutEvent(QFocusEvent *e);
};

#endif // MYLINEEDIT_H

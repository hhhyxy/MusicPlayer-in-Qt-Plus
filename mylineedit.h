#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QLineEdit>
#include <QListWidget>
/*
 * 主界面顶部输入框
 */
class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = nullptr);
    void setDropDownList(QListWidget* theList);
signals:
    // 获得焦点信号
    void focusIn();
    // 失去焦点信号
    void focusOut();
protected:
    bool event(QEvent *e);
    void keyPressEvent(QKeyEvent *event);
    // 获得焦点事件
    void focusInEvent(QFocusEvent * e);
    // 失去焦点事件
    void focusOutEvent(QFocusEvent *e);
private:
    QListWidget *list;
};

#endif // MYLINEEDIT_H

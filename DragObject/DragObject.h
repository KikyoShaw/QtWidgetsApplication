#ifndef MOVEWIDGET_H
#define MOVEWIDGET_H

/**
 * 通用控件移动类
 * 1. 可以指定需要移动的QObject
 * 2. 可设置是否限定鼠标左键拖动
 * 3. 支持任意QObject控件
 */

#include <QWidget>

#ifdef quc
class Q_DECL_EXPORT MoveWidget : public QObject
#else
class DragObject : public QObject
#endif

{
    Q_OBJECT
public:
    explicit DragObject(QObject *parent = 0);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    QPoint lastPoint;   //最后按下的坐标
    bool pressed;       //鼠标是否按下
    bool leftButton;    //限定鼠标左键
    bool inControl;     //限定在容器内
    QWidget *widget = nullptr;    //移动的控件

public Q_SLOTS:
    //设置是否限定鼠标左键
    void setLeftButton(bool leftButton);
    //设置是否限定不能移出容器外面
    void setInControl(bool inControl);
    //设置要移动的控件
    void setWidget(QWidget *widget);
};

#endif // MOVEWIDGET_H

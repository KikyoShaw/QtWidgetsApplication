#pragma once

#include <QScrollBar>
#include <QPropertyAnimation>

class SmoothScrollBar : public QScrollBar
{
    Q_OBJECT
public:
    SmoothScrollBar(QWidget *parent=nullptr);

private:
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
    void mouseMoveEvent(QMouseEvent *) override;

public slots:
    void setValue(int value);
    void scroll(int value);

private:
	QPropertyAnimation *m_scrollAni = nullptr;
	int m_targetValue = 0;

};

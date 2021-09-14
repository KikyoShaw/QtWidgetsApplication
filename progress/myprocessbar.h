#ifndef MYPROCESSBAR_H
#define MYPROCESSBAR_H

#include <QWidget>
#include <QProgressBar>

class QTimer;

class MyProcessBar : public QProgressBar
{
    Q_OBJECT
public:
    explicit MyProcessBar(QWidget *parent = nullptr);
	~MyProcessBar();

private slots:
	void sltTimerOut();

protected:
    void paintEvent(QPaintEvent *event);

private:
    int m_currvalue=0;
    QTimer *m_timer=nullptr;
};

#endif // MYPROCESSBAR_H

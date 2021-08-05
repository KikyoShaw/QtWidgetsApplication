#include "floatingWin.h"
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QScreen>
#include "windows.h"

floatingWin::floatingWin(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//�ƶ�����λ��
	auto primaryScreen = QGuiApplication::primaryScreen();
	if (primaryScreen) {
		auto screenRect = primaryScreen->geometry();
		auto pos = QPoint(screenRect.right() - this->width() - 20, screenRect.top() + 20);
		move(pos);
	}
	//���ô�������
	SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

floatingWin::~floatingWin()
{
}

void floatingWin::enterEvent(QEvent * event)
{
	auto desktop = QApplication::desktop();
	if (desktop) {
		//��ȡ����������Ļλ��
		QRect screenRect = desktop->screenGeometry(this->pos());
		QRect rect = this->geometry();
		if (rect.top() < screenRect.top()) {
			//�ƶ�����λ��
			move(rect.x(), screenRect.top() - 2);
			//���ô�������
			SetWindowPos((HWND)this->winId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}
	setFocus();
	activateWindow();
	QWidget::enterEvent(event);
}

void floatingWin::leaveEvent(QEvent * event)
{
	auto desktop = QApplication::desktop();
	if (desktop) {
		//��ȡ����������Ļλ��
		QRect screenRect = desktop->screenGeometry(this->pos());
		QRect rect = this->geometry();
		if (rect.top() < screenRect.top()) {
			//�ƶ�����λ��
			move(rect.x(), screenRect.top() + 2 - rect.height());
			//���ô�������
			SetWindowPos((HWND)this->winId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}
	}
}

void floatingWin::mouseMoveEvent(QMouseEvent * event)
{
	//�ж�����Ƿ񱻰��£�ֻ����������ˣ��䷵��ֵ����1(true)
	if ((event->buttons() & Qt::LeftButton) && m_bMove)
	{
		move(event->globalPos() - m_point);
	}
	QWidget::mouseMoveEvent(event);
}

void floatingWin::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMove = true;
		m_point = event->globalPos() - frameGeometry().topLeft();
	}
	QWidget::mousePressEvent(event);
}

void floatingWin::mouseReleaseEvent(QMouseEvent * event)
{
	m_bMove = false;
	QWidget::mouseReleaseEvent(event);
}

#include "moveWidget.h"
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>

MoveWidget::MoveWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setAttribute(Qt::WA_TransparentForMouseEvents);

	m_MoveOut.setTimerType(Qt::PreciseTimer);
	m_MoveOut.setInterval(8);
	connect(&m_MoveOut, &QTimer::timeout, this, [this] {
		QPoint pos = QPoint(m_posX, m_posY);
		move(pos);
		if (m_Direction) { //����
			m_posX -= 1; //�ƶ��ٶ�
		}
		else { //����
			m_posX += 1;
		}
		//����߽����⣬���ô����ƶ�����һ�㴰��
		if (m_posX < 0)
		{
			if (m_posX < -1)
				move(QPoint(0, m_posY));
			m_posX = 0;
			m_Direction = false;
		}

		if (parentWidget()) {
			if ((m_posX + this->width()) > parentWidget()->width()) {
				m_posX = parentWidget()->width() - this->width();
				move(QPoint(m_posX, m_posY));
				m_Direction = true;
			}
		}
	});
}

MoveWidget::~MoveWidget()
{
}

void MoveWidget::toStartMoveTimer(bool s, int x, int y)
{
	m_Direction = s;
	m_posX = x;
	m_posY = y;
	//connect(&m_threadFloat, SIGNAL(started()), &m_MoveOut, SLOT(start()), Qt::UniqueConnection);
	//connect(&m_threadFloat, SIGNAL(finished()), &m_MoveOut, SLOT(stop()), Qt::UniqueConnection);
	//m_MoveOut.moveToThread(&m_threadFloat);
	//m_threadFloat.start();
	m_MoveOut.start();
}

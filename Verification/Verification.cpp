#include "Verification.h"
#include <QPainter>
#include <QTime>
#include <QTimer>

Verification::Verification(QWidget *parent)
	: QWidget(parent), m_verificationCode(QString()), m_codeNum(4)
{
	qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
	m_verificationCode = getVerificationCodeByRand();
	m_colors = getColors();

	//m_updateTimer = new QTimer(this);
	//if (m_updateTimer) {
	//	connect(m_updateTimer, &QTimer::timeout, this, &Verification::sltTimerOut);
	//	m_updateTimer->start(1000);
	//}
}

Verification::~Verification()
{
}

void Verification::sltTimerOut()
{
	qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
	m_colors = getColors();
	update();
}

QString Verification::getVerificationCodeByRand()
{
	QString destCode = QString();
	for (int i = 0; i < m_codeNum; i++) {
		int flag = qrand() % 2;
		if (0 == flag) {
			int c = '0' + qrand() % 10;
			destCode += static_cast<QChar>(c);
		}
		else {
			int c = (qrand() % 2) ? 'a' : 'A';
			destCode += static_cast<QChar>(c + qrand() % 26);
		}
	}
	return destCode;
}

Qt::GlobalColor * Verification::getColors()
{
	static Qt::GlobalColor colors[4];
	for (int i = 0; i < 4; i++)
	{
		colors[i] = static_cast<Qt::GlobalColor>(2 + qrand() % 16);
	}
	return colors;
}

void Verification::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	//填充验证码绘制矩形
	painter.fillRect(0, 0, 100, 30, QColor(255, 250, 240));
	painter.setFont(QFont("Comic Sans MS", 12));

	//绘制验证码
	for (int i = 0; i < m_codeNum; i++)
	{
		painter.setPen(m_colors[i]);
		painter.drawText(25 * i, 0, 25, 30, Qt::AlignCenter, QString(m_verificationCode[i]));
	}

	//绘制噪点
	if (m_bstyle == BackgroundStyle::E_DOT)
		paintDot(&painter);
}

void Verification::paintDot(QPainter * painter)
{
	if (Q_NULLPTR == painter)
		return;

	//绘制噪点
	for (int i = 0; i < 150; i++)
	{
		painter->setPen(m_colors[i % 4]);
		painter->drawPoint(qrand() % 99, qrand() % 29);
	}
}

void Verification::mouseDoubleClickEvent(QMouseEvent * event)
{
	m_verificationCode = getVerificationCodeByRand();
	m_colors = getColors();
	update();
	QWidget::mouseDoubleClickEvent(event);
}

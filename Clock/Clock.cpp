#include "Clock.h"
#include <QPainter>
#include <QDate>
#include <QTimer>
#include <QKeyEvent>

Clock::Clock(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	//��ʼ������
	initData();

	m_time = new QTimer(this);
	if (m_time) {
		//ÿ 1s ˢ����Ļ ����ӵ���paintEvent�¼�
		connect(m_time, SIGNAL(timeout()), this, SLOT(update()));
		m_time->start(1000);
	}

	this->showFullScreen();
}

Clock::~Clock()
{
}

void Clock::initData()
{
	//����״̬
	strListState.append(QStringLiteral("����"));
	strListState.append(QStringLiteral("����"));
	strListState.append(QStringLiteral("����"));
	strListState.append(QStringLiteral("����"));
	//��������
	strListWeek.append(QStringLiteral("��һ"));
	strListWeek.append(QStringLiteral("�ܶ�"));
	strListWeek.append(QStringLiteral("����"));
	strListWeek.append(QStringLiteral("����"));
	strListWeek.append(QStringLiteral("����"));
	strListWeek.append(QStringLiteral("����"));
	strListWeek.append(QStringLiteral("����"));
	//��������
	strListDate.append(QStringLiteral("һ"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("��"));
	strListDate.append(QStringLiteral("ʮ"));
	strListDate.append(QStringLiteral("ʮһ"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("ʮ��"));
	strListDate.append(QStringLiteral("��ʮ"));
	strListDate.append(QStringLiteral("��ʮһ"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ"));
	strListDate.append(QStringLiteral("��ʮһ"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ"));
	strListDate.append(QStringLiteral("��ʮһ"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ"));
	strListDate.append(QStringLiteral("��ʮһ"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��ʮ��"));
	strListDate.append(QStringLiteral("��"));
}

double Clock::getMaxDate(int y, int m)
{
	int loopDay = isLoopYear(y) ? 1 : 0;
	switch (m)
	{
	case 1:
	case 2:
		return (double)28 + loopDay;
		break;
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31.0;
		break;
	}
	return 30.0;
}

int Clock::getWeek(QString str, QStringList list)
{
	for (int i = 0; i < 7; i++)
	{
		if (str == list[i])
		{
			return i;
		}
	}
	return 0;
}

QString Clock::getYear(int y)
{
	QString sYear = QString::number(y);
	QString newsYear = QString();
	for (int i = 0; i < sYear.size(); i++)
	{
		int s = QString("%1").arg(sYear[i]).toInt();
		switch (s)
		{
		case 0:
			newsYear += QStringLiteral("��");
			break;
		case 1:
			newsYear += QStringLiteral("һ");
			break;
		case 2:
			newsYear += QStringLiteral("��");
			break;
		case 3:
			newsYear += QStringLiteral("��");
			break;
		case 4:
			newsYear += QStringLiteral("��");
			break;
		case 5:
			newsYear += QStringLiteral("��");
			break;
		case 6:
			newsYear += QStringLiteral("��");
			break;
		case 7:
			newsYear += QStringLiteral("��");
			break;
		case 8:
			newsYear += QStringLiteral("��");
			break;
		case 9:
			newsYear += QStringLiteral("��");
			break;
		}
	}
	newsYear += QStringLiteral("��");
	return newsYear;
}

bool Clock::isLoopYear(int year)
{
	return (((0 == (year % 4)) && (0 != (year % 100))) || (0 == (year % 400)));
}

void Clock::paintEvent(QPaintEvent * event)
{
	Q_UNUSED(event);
	 
	QPainter painter(this);
	// �����
	painter.setRenderHints(QPainter::Antialiasing, true);
	// ͼƬƽ������
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	painter.drawPixmap(0, 0, QPixmap(":/Clock/qrc/bg.jpg").scaled(this->width(), (this->height() == 0 ? this->width() : this->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	QFont font;
	//ÿ����ת�Ƕ�
	double rotate;      
	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();
	QDateTime dateTime = QDateTime::currentDateTime();

	//��������
	font.setPointSize(12);
	font.setFamily(QStringLiteral("����"));
	//��������
	painter.setPen(Qt::white);
	painter.setFont(font);
	//��Χ����
	//����Բ��
	painter.translate(this->width() / 2, this->height() / 2);
	//����һ���߽�,��֤��Բ�ε�
	int side = qMin(width(), height());
	painter.scale(side / 1200.0, side / 1200.0);

	//��ʼ��ͼ
	//���
	auto _year = date.year();
	painter.drawText(QPoint(-50, 0), getYear(_year));
	//�·�
	painter.rotate((1 - date.month()) * 30);
	for (int i = 0; i < 12; i++)
	{
		if ((i + 1) == date.month())
		{
			painter.drawText(QPoint(90, 0), strListDate[i] + QStringLiteral("��"));
			painter.rotate(30);
			continue;
		}
		painter.drawText(QPoint(100, 0), strListDate[i]);
		painter.rotate(30);
	}
	//����
	rotate = 360.0 / getMaxDate(_year, date.month());
	painter.rotate((date.month() - 1) * 30);			//���Ƕ�����
	painter.rotate((1 - date.day())*rotate);			//����ʹ��ǰ������x����

	for (int i = 0; i < (int)getMaxDate(_year, date.month()); i++)
	{
		if ((i + 1) == date.day())
		{
			painter.drawText(QPoint(150, 0), strListDate[i] + QStringLiteral("��"));
			painter.rotate(rotate);
			continue;
		}
		painter.drawText(QPoint(150, 0), strListDate[i]);
		painter.rotate(rotate);
	}
	//����
	painter.rotate((date.day() - 1)*rotate);			//���Ƕ�����
	QString tempStr = dateTime.toString().left(2);	//�ҵ��� *
	painter.rotate(-getWeek(tempStr, strListWeek)*360.0 / 7.0);	//��ʱ����ת
	for (int i = 0; i < 7; i++)
	{
		painter.drawText(QPoint(210, 0), strListWeek[i]);
		painter.rotate(360.0 / 7.0);
	}
	//״̬
	double initRotate;										//״̬
	painter.rotate(getWeek(tempStr, strListWeek)*360.0 / 7.0);	//���Ƕ�����
	if (time.hour() >= 6 && time.hour() < 12)	initRotate = 0;
	if (time.hour() >= 12 && time.hour() < 14)	initRotate = -1;
	if (time.hour() >= 14 && time.hour() < 19)	initRotate = -2;
	if (time.hour() >= 19 || time.hour() < 6)	initRotate = -3;
	painter.rotate(initRotate * 90);				//��ʼ��״̬
	for (int i = 0; i < 4; i++)
	{
		painter.drawText(QPoint(280, 0), strListState[i]);
		painter.rotate(90);
	}
	//Сʱ
	painter.rotate(-initRotate * 90);				//���Ƕ�����
	painter.rotate((1 - time.hour()) * 30);			//��ʼ��״̬
	for (int i = 0; i < 12; i++)
	{
		if (time.hour() == (i + 1) || (time.hour() - 12) == (i + 1))
		{
			painter.drawText(QPoint(390, 0), strListDate[i] + QStringLiteral("��"));
			painter.rotate(30);
			continue;
		}
		painter.drawText(QPoint(390, 0), strListDate[i]);
		painter.rotate(30);
	}
	//����
	painter.rotate((-1 + time.hour()) * 30);			//������תǰ�ĽǶ�
	painter.rotate((1 - time.minute()) * 6);			//��ʼ��״̬
	for (int i = 0; i < 60; i++)
	{
		if (time.minute() == (i + 1))
		{
			painter.drawText(QPoint(450, 0), strListDate[i] + QStringLiteral("��"));
			painter.rotate(6);
			continue;
		}
		painter.drawText(QPoint(450, 0), strListDate[i]);
		painter.rotate(6);
	}
	//����
	painter.rotate((time.minute() - 1) * 6);					//������תǰ�ĽǶ�
	painter.rotate((1 - time.second()) * 6);					//��ʼ��״̬
	for (int i = 0; i < 60; i++)
	{
		if (time.second() == (i + 1))
		{
			painter.drawText(QPoint(530, 0), strListDate[i] + QStringLiteral("��"));
			painter.rotate(6);
			continue;
		}
		painter.drawText(QPoint(530, 0), strListDate[i]);
		painter.rotate(6);
	}
}

void Clock::mouseDoubleClickEvent(QMouseEvent * event)
{
	Q_UNUSED(event);
	if (this->isFullScreen())
	{
		this->showNormal();
	}
	else
	{
		this->showFullScreen();
	}
	QWidget::mouseDoubleClickEvent(event);
}

void Clock::keyPressEvent(QKeyEvent * event)
{
	Q_UNUSED(event);

	if (event->key() == Qt::Key_Escape)
	{
		this->close();
	}

	QWidget::keyPressEvent(event);
}

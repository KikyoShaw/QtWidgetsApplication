#include "Clock.h"
#include <QPainter>
#include <QDate>
#include <QTimer>
#include <QKeyEvent>

Clock::Clock(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	//初始化数据
	initData();

	m_time = new QTimer(this);
	if (m_time) {
		//每 1s 刷新屏幕 ，间接调用paintEvent事件
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
	//加载状态
	strListState.append(QStringLiteral("上午"));
	strListState.append(QStringLiteral("中午"));
	strListState.append(QStringLiteral("下午"));
	strListState.append(QStringLiteral("晚上"));
	//加载星期
	strListWeek.append(QStringLiteral("周一"));
	strListWeek.append(QStringLiteral("周二"));
	strListWeek.append(QStringLiteral("周三"));
	strListWeek.append(QStringLiteral("周四"));
	strListWeek.append(QStringLiteral("周五"));
	strListWeek.append(QStringLiteral("周六"));
	strListWeek.append(QStringLiteral("周日"));
	//加载日期
	strListDate.append(QStringLiteral("一"));
	strListDate.append(QStringLiteral("二"));
	strListDate.append(QStringLiteral("三"));
	strListDate.append(QStringLiteral("四"));
	strListDate.append(QStringLiteral("五"));
	strListDate.append(QStringLiteral("六"));
	strListDate.append(QStringLiteral("七"));
	strListDate.append(QStringLiteral("八"));
	strListDate.append(QStringLiteral("九"));
	strListDate.append(QStringLiteral("十"));
	strListDate.append(QStringLiteral("十一"));
	strListDate.append(QStringLiteral("十二"));
	strListDate.append(QStringLiteral("十三"));
	strListDate.append(QStringLiteral("十四"));
	strListDate.append(QStringLiteral("十五"));
	strListDate.append(QStringLiteral("十六"));
	strListDate.append(QStringLiteral("十七"));
	strListDate.append(QStringLiteral("十八"));
	strListDate.append(QStringLiteral("十九"));
	strListDate.append(QStringLiteral("二十"));
	strListDate.append(QStringLiteral("二十一"));
	strListDate.append(QStringLiteral("二十二"));
	strListDate.append(QStringLiteral("二十三"));
	strListDate.append(QStringLiteral("二十四"));
	strListDate.append(QStringLiteral("二十五"));
	strListDate.append(QStringLiteral("二十六"));
	strListDate.append(QStringLiteral("二十七"));
	strListDate.append(QStringLiteral("二十八"));
	strListDate.append(QStringLiteral("二十九"));
	strListDate.append(QStringLiteral("三十"));
	strListDate.append(QStringLiteral("三十一"));
	strListDate.append(QStringLiteral("三十二"));
	strListDate.append(QStringLiteral("三十三"));
	strListDate.append(QStringLiteral("三十四"));
	strListDate.append(QStringLiteral("三十五"));
	strListDate.append(QStringLiteral("三十六"));
	strListDate.append(QStringLiteral("三十七"));
	strListDate.append(QStringLiteral("三十八"));
	strListDate.append(QStringLiteral("三十九"));
	strListDate.append(QStringLiteral("四十"));
	strListDate.append(QStringLiteral("四十一"));
	strListDate.append(QStringLiteral("四十二"));
	strListDate.append(QStringLiteral("四十三"));
	strListDate.append(QStringLiteral("四十四"));
	strListDate.append(QStringLiteral("四十五"));
	strListDate.append(QStringLiteral("四十六"));
	strListDate.append(QStringLiteral("四十七"));
	strListDate.append(QStringLiteral("四十八"));
	strListDate.append(QStringLiteral("四十九"));
	strListDate.append(QStringLiteral("五十"));
	strListDate.append(QStringLiteral("五十一"));
	strListDate.append(QStringLiteral("五十二"));
	strListDate.append(QStringLiteral("五十三"));
	strListDate.append(QStringLiteral("五十四"));
	strListDate.append(QStringLiteral("五十五"));
	strListDate.append(QStringLiteral("五十六"));
	strListDate.append(QStringLiteral("五十七"));
	strListDate.append(QStringLiteral("五十八"));
	strListDate.append(QStringLiteral("五十九"));
	strListDate.append(QStringLiteral("零"));
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
			newsYear += QStringLiteral("零");
			break;
		case 1:
			newsYear += QStringLiteral("一");
			break;
		case 2:
			newsYear += QStringLiteral("二");
			break;
		case 3:
			newsYear += QStringLiteral("三");
			break;
		case 4:
			newsYear += QStringLiteral("四");
			break;
		case 5:
			newsYear += QStringLiteral("五");
			break;
		case 6:
			newsYear += QStringLiteral("六");
			break;
		case 7:
			newsYear += QStringLiteral("七");
			break;
		case 8:
			newsYear += QStringLiteral("八");
			break;
		case 9:
			newsYear += QStringLiteral("九");
			break;
		}
	}
	newsYear += QStringLiteral("年");
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
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	painter.drawPixmap(0, 0, QPixmap(":/Clock/qrc/bg.jpg").scaled(this->width(), (this->height() == 0 ? this->width() : this->height()), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	QFont font;
	//每次旋转角度
	double rotate;      
	QDate date = QDate::currentDate();
	QTime time = QTime::currentTime();
	QDateTime dateTime = QDateTime::currentDateTime();

	//字体设置
	font.setPointSize(12);
	font.setFamily(QStringLiteral("楷体"));
	//文字设置
	painter.setPen(Qt::white);
	painter.setFont(font);
	//范围设置
	//设置圆点
	painter.translate(this->width() / 2, this->height() / 2);
	//定义一个边界,保证是圆形的
	int side = qMin(width(), height());
	painter.scale(side / 1200.0, side / 1200.0);

	//开始画图
	//年份
	auto _year = date.year();
	painter.drawText(QPoint(-50, 0), getYear(_year));
	//月份
	painter.rotate((1 - date.month()) * 30);
	for (int i = 0; i < 12; i++)
	{
		if ((i + 1) == date.month())
		{
			painter.drawText(QPoint(90, 0), strListDate[i] + QStringLiteral("月"));
			painter.rotate(30);
			continue;
		}
		painter.drawText(QPoint(100, 0), strListDate[i]);
		painter.rotate(30);
	}
	//日期
	rotate = 360.0 / getMaxDate(_year, date.month());
	painter.rotate((date.month() - 1) * 30);			//将角度置零
	painter.rotate((1 - date.day())*rotate);			//调节使当前日期在x轴上

	for (int i = 0; i < (int)getMaxDate(_year, date.month()); i++)
	{
		if ((i + 1) == date.day())
		{
			painter.drawText(QPoint(150, 0), strListDate[i] + QStringLiteral("号"));
			painter.rotate(rotate);
			continue;
		}
		painter.drawText(QPoint(150, 0), strListDate[i]);
		painter.rotate(rotate);
	}
	//星期
	painter.rotate((date.day() - 1)*rotate);			//将角度置零
	QString tempStr = dateTime.toString().left(2);	//找到周 *
	painter.rotate(-getWeek(tempStr, strListWeek)*360.0 / 7.0);	//逆时针旋转
	for (int i = 0; i < 7; i++)
	{
		painter.drawText(QPoint(210, 0), strListWeek[i]);
		painter.rotate(360.0 / 7.0);
	}
	//状态
	double initRotate;										//状态
	painter.rotate(getWeek(tempStr, strListWeek)*360.0 / 7.0);	//将角度置零
	if (time.hour() >= 6 && time.hour() < 12)	initRotate = 0;
	if (time.hour() >= 12 && time.hour() < 14)	initRotate = -1;
	if (time.hour() >= 14 && time.hour() < 19)	initRotate = -2;
	if (time.hour() >= 19 || time.hour() < 6)	initRotate = -3;
	painter.rotate(initRotate * 90);				//初始化状态
	for (int i = 0; i < 4; i++)
	{
		painter.drawText(QPoint(280, 0), strListState[i]);
		painter.rotate(90);
	}
	//小时
	painter.rotate(-initRotate * 90);				//将角度置零
	painter.rotate((1 - time.hour()) * 30);			//初始化状态
	for (int i = 0; i < 12; i++)
	{
		if (time.hour() == (i + 1) || (time.hour() - 12) == (i + 1))
		{
			painter.drawText(QPoint(390, 0), strListDate[i] + QStringLiteral("点"));
			painter.rotate(30);
			continue;
		}
		painter.drawText(QPoint(390, 0), strListDate[i]);
		painter.rotate(30);
	}
	//分钟
	painter.rotate((-1 + time.hour()) * 30);			//返回旋转前的角度
	painter.rotate((1 - time.minute()) * 6);			//初始化状态
	for (int i = 0; i < 60; i++)
	{
		if (time.minute() == (i + 1))
		{
			painter.drawText(QPoint(450, 0), strListDate[i] + QStringLiteral("分"));
			painter.rotate(6);
			continue;
		}
		painter.drawText(QPoint(450, 0), strListDate[i]);
		painter.rotate(6);
	}
	//秒钟
	painter.rotate((time.minute() - 1) * 6);					//返回旋转前的角度
	painter.rotate((1 - time.second()) * 6);					//初始化状态
	for (int i = 0; i < 60; i++)
	{
		if (time.second() == (i + 1))
		{
			painter.drawText(QPoint(530, 0), strListDate[i] + QStringLiteral("秒"));
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

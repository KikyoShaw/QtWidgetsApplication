#include "Lrc.h"
#include <QPainter>
#include <QTimer>

Lrc::Lrc(QWidget *parent)
    : QLabel(parent), m_lrcWidth(1000),
	m_lrcMaskWidth(0), m_lrcMaskWidthInterval(0)
{
    ui.setupUi(this);

	//����͸��ȥ���߿�
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	//���ô�С
	setFixedSize(m_lrcWidth, 60);

	//��ʼ������
	m_font.setFamily("Times New Roman");
	m_font.setBold(true);
	m_font.setPointSize(30);

	//��ʼ����ʵ����Խ��������
	m_textLinearGradient.setStart(0, 10);
	m_textLinearGradient.setFinalStop(0, 40);
	m_textLinearGradient.setColorAt(0.1, QColor(14, 179, 255));
	m_textLinearGradient.setColorAt(0.5, QColor(114, 232, 255));
	m_textLinearGradient.setColorAt(0.9, QColor(14, 179, 255));

	//��ʼ�����ֵ����Խ��������
	m_maskLinearGradient.setStart(0, 10);
	m_maskLinearGradient.setFinalStop(0, 40);
	m_maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
	m_maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
	m_maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));

	// ��ʱ����ʼ��
	m_timer = new QTimer(this);
	if (m_timer) {
		connect(m_timer, &QTimer::timeout, this, &Lrc::sltTimerOut);
	}

	startLrcMask(6000, QStringLiteral("���ֺν� ����ʶ����� ���Ҷ�ȱ ��һ�����˽�"));
}

Lrc::~Lrc()
{
}

void Lrc::setLrcTextFontSize(int size)
{
	m_font.setPointSize(size);
}

void Lrc::startLrcMask(qint64 intervalTime, const QString& text)
{
	//�����ı�����
	setText(text);
	// ��������ÿ��30�������һ�����ֵĿ�ȣ���Ϊ�������̫Ƶ��
   // ������CPUռ���ʣ������ʱ����̫���򶯻�Ч���Ͳ�������
	qreal count = intervalTime / 30;
	// ��ȡ����ÿ����Ҫ���ӵĿ�ȣ������m_lrcWidth�ǲ����Ĺ̶����
	m_lrcMaskWidthInterval = m_lrcWidth / count;
	m_lrcMaskWidth = 0;
	if (m_timer) {
		m_timer->start(30);
	}
}

void Lrc::stopLrcMask()
{
	if (m_timer) {
		m_timer->stop();
	}
	m_lrcMaskWidth = 0;
	update();
}

void Lrc::sltTimerOut()
{
	m_lrcMaskWidth += m_lrcMaskWidthInterval;
	update();
}

void Lrc::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	painter.setFont(m_font);
	// �Ȼ��Ƶײ����֣���Ϊ��Ӱ��������ʹ��ʾЧ�������������Ҹ����ʸ�
	painter.setPen(QColor(0, 0, 0, 200));
	painter.drawText(1, 1, m_lrcWidth, 60, Qt::AlignLeft, text());
	// ����������ƽ�������
	painter.setPen(QPen(m_textLinearGradient, 0));
	painter.drawText(0, 0, m_lrcWidth, 60, Qt::AlignLeft, text());
	// ���ø������
	painter.setPen(QPen(m_maskLinearGradient, 0));
	painter.drawText(0, 0, m_lrcMaskWidth, 60, Qt::AlignLeft, text());
}

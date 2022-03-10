#pragma once

#include <QWidget>

enum BackgroundStyle
{
	E_NONE = 0,
	E_DOT = 1,
};

class Verification : public QWidget
{
	Q_OBJECT

public:
	Verification(QWidget *parent = Q_NULLPTR);
	~Verification();

	//������ʽ
	 void setBackgroundStyle(BackgroundStyle style) { m_bstyle = style;}
	 //��ȡ��ǰ�����֤��
	 QString getVerificationCode() { return m_verificationCode; }

private slots:
	void sltTimerOut();

private:
	//��ȡ�����֤��
	QString getVerificationCodeByRand();
	//��ȡ��ɫ
	Qt::GlobalColor* getColors();

private:
	virtual void paintEvent(QPaintEvent *event);
	void paintDot(QPainter *painter);

	virtual void mouseDoubleClickEvent(QMouseEvent *event);

private:
	QString m_verificationCode; //��֤���ַ���
	int m_codeNum; //��֤���ַ�������
	Qt::GlobalColor *m_colors = nullptr; //��ɫ��ʽ,��Ϊ�����ɫ���ܻᵼ���ַ���ʾ���壬��˽���ѡ��һЩ��ɫ�ŵ����������ѡ��
	BackgroundStyle m_bstyle = BackgroundStyle::E_NONE; //���Ʊ�����ʽ
	QTimer *m_updateTimer = nullptr; //��ʱ��ˢ�£�Ŀǰ�Ȳ��ã��ݲ��ԣ�
};

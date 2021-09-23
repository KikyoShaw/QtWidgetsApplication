#pragma once

#include <QWidget>

class QPainter;


enum DateProperty
{
	E_Property_None, //ͨ��
	E_Property_Day = 1, //���ڿؼ�
	E_Property_Month = 2, //�·ݿؼ�
	E_Property_Year = 3, //��ݿؼ�
};

enum ScrollType 
{
	E_VERTICAL = 1, //��ֱ��ʾ
	E_HORIZONTAL = 2, //ˮƽ��ʾ
};

class RollingBox : public QWidget
{
	Q_OBJECT

public:
	explicit RollingBox(QWidget *parent = 0);
	~RollingBox();
	//��ȡ�м�ֵ
	inline int getValue() { return m_nCurrentValue; }
	//�����м�ֵ
	inline void setValue(int nValue) { m_nCurrentValue = nValue; }
	//���ù����Ĳ���
	inline void setStep(int nStep) { m_nStep = nStep; }
	//������ʾ����
	inline void setDevice(int nDevice) { m_nDevice = nDevice; }
	//������ʾ�߽�
	void setRang(int nMin, int nMax);
	//��������
	void setPropertys(DateProperty date) { m_dateProperty = date; };
	//ˮƽ���Ǵ�ֱ����
	void setScrollType(ScrollType types) { m_scrollType = types; };

protected:
	void wheelEvent(QWheelEvent* event);
	void paintEvent(QPaintEvent* event);
	//�����м�ѡ��
	void paintLine(QPainter* pPainter);
	//��������
	void paintText(QPainter* pPainter, int nValue, int nOffSet, int nFontSize);

	//��ȡ�İ�
	QString getTextByProperty(const QString &text);

	//��ȡ��ʽ
	QColor getTextStyle(int nOffSet);

signals:
	void sigCurrentValueChange(int nValue, DateProperty dateProperty);

private:
	//��ǰѡ��ֵ
	int m_nCurrentValue;
	//ƫ��ֵ
	int m_nOffSet;
	//���������ֵ
	int m_nMax;
	//��������Сֵ
	int m_nMin;
	//��ʾ������
	int m_nDevice;
	//�����Ĳ���
	int m_nStep;
	//����
	DateProperty m_dateProperty;
	//ˮƽ/��ֱ����
	ScrollType m_scrollType;
};
#pragma once

#include <QScrollBar>

/*
* ������������
*/

class SuspensionScrollBar : public QScrollBar
{
	Q_OBJECT
public:
	explicit SuspensionScrollBar(Qt::Orientation type = Qt::Vertical, QWidget *parent = Q_NULLPTR);
	~SuspensionScrollBar();

	//���û�������ʽ
	void setScrollBarStyleSheet(const QString& style);

	//ͨ��QSS�ļ�ֱ�����û�������ʽ
	void setScrollBarStyleSheetByQSS(const QString& qssPath);

public slots:
	void sltValueChanged(int value); //�޸Ļ�������ǰ��ʾλ��
	void sltRangeChanged(int min, int max); //�޸Ļ�������ʾ�ߴ�
};

/*
* QListView
*/

#include <QListView>

class ScrollBarListView : public QListView
{
	Q_OBJECT
public:
	explicit ScrollBarListView(QWidget *parent = Q_NULLPTR);
	~ScrollBarListView();

	void setVerticalScrollBarVisible(bool visible); //�Ƿ���ʾ��ֱ������
	void setHorizontalScrollBarVisible(bool visible); //�Ƿ���ʾˮƽ������

private slots:
	void sltVerticalValueChanged(int value); //��ֱ����λ��
	void sltHorizontalValueChanged(int value); //ˮƽ����λ��

private:
	virtual void resizeEvent(QResizeEvent *event);

private:
	SuspensionScrollBar *m_verticalScrollBar = Q_NULLPTR; //��ֱ��������
	SuspensionScrollBar *m_horizontalScrollBar = Q_NULLPTR; //ˮƽ��������
};

/*
* QListWidget
*/

#include <QListWidget>

class ScrollBarListWidget : public QListWidget
{
	Q_OBJECT
public:
	explicit ScrollBarListWidget(QWidget *parent = Q_NULLPTR);
	~ScrollBarListWidget();

	void setVerticalScrollBarVisible(bool visible); //�Ƿ���ʾ��ֱ������
	void setHorizontalScrollBarVisible(bool visible); //�Ƿ���ʾˮƽ������

private slots:
	void sltVerticalValueChanged(int value); //��ֱ����λ��
	void sltHorizontalValueChanged(int value); //ˮƽ����λ��

private:
	virtual void resizeEvent(QResizeEvent *event);

private:
	SuspensionScrollBar *m_verticalScrollBar = Q_NULLPTR; //��ֱ��������
	SuspensionScrollBar *m_horizontalScrollBar = Q_NULLPTR; //ˮƽ��������
};

/*
* QScrollArea
*/
#include <QScrollArea>

class ScrollBarScrollArea : public QScrollArea
{
	Q_OBJECT
public:
	explicit ScrollBarScrollArea(QWidget *parent = Q_NULLPTR);
	~ScrollBarScrollArea();

	void setVerticalScrollBarVisible(bool visible); //�Ƿ���ʾ��ֱ������
	void setHorizontalScrollBarVisible(bool visible); //�Ƿ���ʾˮƽ������

private slots:
	void sltVerticalValueChanged(int value); //��ֱ����λ��
	void sltHorizontalValueChanged(int value); //ˮƽ����λ��

private:
	virtual void resizeEvent(QResizeEvent *event);

private:
	SuspensionScrollBar *m_verticalScrollBar = Q_NULLPTR; //��ֱ��������
	SuspensionScrollBar *m_horizontalScrollBar = Q_NULLPTR; //ˮƽ��������
};




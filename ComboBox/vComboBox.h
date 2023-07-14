#pragma once

#include <QComboBox>

class vComboBox : public QComboBox
{
	Q_OBJECT

public:
	vComboBox(QWidget *parent = nullptr);
	~vComboBox();

	//设置下拉框位置
	void setPopupLocation(int x, int y);

public:
	virtual void showPopup();
	virtual void hidePopup();
	virtual bool eventFilter(QObject *obj, QEvent *event);

signals:
	void sigPopup();
	void sigHidePopup();

private:
	int m_posX = 0;
	int m_posY = 12;
};

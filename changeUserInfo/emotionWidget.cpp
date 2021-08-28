#include "emotionWidget.h"
#include <QButtonGroup>

EmotionWidget::EmotionWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
}

EmotionWidget::~EmotionWidget()
{
}

void EmotionWidget::initButtonGroup()
{
	m_buttonGroup = new QButtonGroup(this);
	if (m_buttonGroup) {
		m_buttonGroup->setExclusive(true);
		m_buttonGroup->addButton(ui.pushButton_1);
		m_buttonGroup->addButton(ui.pushButton_2);
		m_buttonGroup->addButton(ui.pushButton_3);
		m_buttonGroup->addButton(ui.pushButton_4);
		m_buttonGroup->addButton(ui.pushButton_5);
		typedef void (QButtonGroup::*buttonClicked)(QAbstractButton *, bool);
		connect(m_buttonGroup, static_cast<buttonClicked>(&QButtonGroup::buttonToggled), this, &EmotionWidget::sltButtonToggled);
	}
}

void EmotionWidget::sltButtonToggled(QAbstractButton * button, bool toggled)
{
	if (!toggled) {
		return;
	}
	if (button == Q_NULLPTR) {
		return;
	}

	//获取当前选中是男还是女
	bool ret = false;
	//auto result = button->property(Property_sex).toInt(&ret);
	if (ret) {

	}
}

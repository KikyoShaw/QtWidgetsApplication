#include "emotionWidget.h"
#include <QButtonGroup>
#include <QAbstractButton>

constexpr char* Property_Emotion = "EmotionValue";

EmotionWidget::EmotionWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	//初始化button项
	initButtonGroup();
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

	ui.pushButton_1->setProperty(Property_Emotion, E_Secret);
	ui.pushButton_2->setProperty(Property_Emotion, E_Single);
	ui.pushButton_3->setProperty(Property_Emotion, E_InLove);
	ui.pushButton_4->setProperty(Property_Emotion, E_Married);
	ui.pushButton_5->setProperty(Property_Emotion, E_SameSex);
}

void EmotionWidget::sltButtonToggled(QAbstractButton * button, bool toggled)
{
	if (!toggled) {
		return;
	}
	if (button == Q_NULLPTR) {
		return;
	}

	//获取当前选中项属性
	bool ret = false;
	auto result = button->property(Property_Emotion).toInt(&ret);
	if (ret) {
		emit sigSelectIndex(result);
	}
}

#include "baseInfoWidget.h"
#include <QButtonGroup>
#include "emotionWidget.h"

constexpr char* Property_sex = "SEX";

BaseInfoWidget::BaseInfoWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent)
{
	ui.setupUi(this);

	initWidgets();

	//限制器
	ui.lineEdit_job->setMaxLength(8);
	ui.lineEdit_nickName->setMaxLength(12);

	//职业计数
	connect(ui.lineEdit_job, &QLineEdit::textChanged, this, [=]() {
		auto _length = ui.lineEdit_job->text().size();
		ui.label_jobNum->setText(QString("%1/8").arg(QString::number(_length)));
	});

	//打开情感下拉框
	connect(ui.label_emotion, &vLabel::sigRightMouseClicked, this, &BaseInfoWidget::sltOpenEmotionWidget);
	connect(ui.label_emotion, &vLabel::sigFocusOut, this, [=]() {
		if (m_emotionWidget) {
			m_emotionWidget->setVisible(false);
		}
	});
}

BaseInfoWidget::~BaseInfoWidget()
{
}

void BaseInfoWidget::clearUI()
{
	ui.label_birth->clearFocus();
	ui.label_emotion->clearFocus();
	ui.lineEdit_job->clearFocus();
	ui.lineEdit_nickName->clearFocus();
	if (m_emotionWidget) {
		m_emotionWidget->setVisible(false);
	}
}

void BaseInfoWidget::initButtonGroup()
{
	m_RButtonGroup = new QButtonGroup(this);
	if (m_RButtonGroup) {
		m_RButtonGroup->addButton(ui.radioButton_man);
		m_RButtonGroup->addButton(ui.radioButton_woman);
		typedef void (QButtonGroup::*buttonClicked)(QAbstractButton *, bool);
		connect(m_RButtonGroup, static_cast<buttonClicked>(&QButtonGroup::buttonToggled), this, &BaseInfoWidget::sltButtonToggled);
	}
	//赋值
	ui.radioButton_man->setProperty(Property_sex, E_SEX_MAN);
	ui.radioButton_woman->setProperty(Property_sex, E_SEX_WOMAN);
}

void BaseInfoWidget::initWidgets()
{
	//情感状态下拉框
	m_emotionWidget = new EmotionWidget(this);
	if (m_emotionWidget) {
		m_emotionWidget->setVisible(false);
	}
}

void BaseInfoWidget::sltButtonToggled(QAbstractButton * button, bool toggled)
{
	if (!toggled) {
		return;
	}
	if (button == Q_NULLPTR) {
		return;
	}

	//获取当前选中是男还是女
	bool ret = false;
	auto result = button->property(Property_sex).toInt(&ret);
	if (ret) {
		
	}
}

void BaseInfoWidget::sltOpenEmotionWidget()
{
	if (m_emotionWidget) {
		auto isShow = m_emotionWidget->isVisible();
		if (!isShow) {
			int posX = 80;
			int posY = 218;
			m_emotionWidget->move(mapToGlobal(QPoint(posX, posY)));
		}
		m_emotionWidget->setVisible(!isShow);
	}
}

bool BaseInfoWidget::eventFilter(QObject * obj, QEvent * event)
{
	if (obj == Q_NULLPTR) {
		return false;
	}
	if (QEvent::MouseButtonPress == event->type()) {


	}
	return QWidget::eventFilter(obj, event);
}

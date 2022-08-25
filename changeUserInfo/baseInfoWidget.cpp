#include "baseInfoWidget.h"
#include <QButtonGroup>
#include "emotionWidget.h"
#include "birthInfoWidget.h"

const char* Property_sex = "SEX";

//�������
QMap<EmotionValue, QString> EmotionTitles = {
	{ EmotionValue::E_Secret, QStringLiteral("����") },
	{ EmotionValue::E_Single, QStringLiteral("����") },
	{ EmotionValue::E_InLove, QStringLiteral("������") },
	{ EmotionValue::E_Married, QStringLiteral("�ѻ�") },
	{ EmotionValue::E_SameSex, QStringLiteral("ͬ��") },
};

BaseInfoWidget::BaseInfoWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent)
{
	ui.setupUi(this);

	initWidgets();

	//������
	ui.lineEdit_job->setMaxLength(8);
	ui.lineEdit_nickName->setMaxLength(12);

	//ְҵ����
	connect(ui.lineEdit_job, &QLineEdit::textChanged, this, [=]() {
		auto _length = ui.lineEdit_job->text().size();
		ui.label_jobNum->setText(QString("%1/8").arg(QString::number(_length)));
	});

	//�����������
	connect(ui.label_emotion, &vLabel::sigRightMouseClicked, this, &BaseInfoWidget::sltOpenEmotionWidget);
	connect(ui.label_emotion, &vLabel::sigFocusState, this, &BaseInfoWidget::sltEmotioneFocusState);

	//����������������
	connect(ui.label_birth, &vLabel::sigRightMouseClicked, this, &BaseInfoWidget::sltOpenBirthInfoWidget);
	connect(ui.label_birth, &vLabel::sigFocusState, this, &BaseInfoWidget::sltBirthFocusState);
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
	//��ֵ
	ui.radioButton_man->setProperty(Property_sex, E_SEX_MAN);
	ui.radioButton_woman->setProperty(Property_sex, E_SEX_WOMAN);
}

void BaseInfoWidget::initWidgets()
{
	//���״̬������
	m_emotionWidget = new EmotionWidget(this);
	if (m_emotionWidget) {
		m_emotionWidget->setVisible(false);
		//���ѡ��
		connect(m_emotionWidget, &EmotionWidget::sigSelectIndex, this, &BaseInfoWidget::sltSetEmotion);
	}

	//����������������
	m_birthInfoWidget = new BirthInfoWidget(this);
	if (m_birthInfoWidget) {
		m_birthInfoWidget->setVisible(false);
		connect(m_birthInfoWidget, &BirthInfoWidget::sigDateValueChange, this, &BaseInfoWidget::sltSetBirthInfo);
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

	//��ȡ��ǰѡ�����л���Ů
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
			ui.label_emotionIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_sq.png);");
		}
		else {
			ui.label_emotionIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_zk.png);");
		}
		m_emotionWidget->setVisible(!isShow);
	}
}

void BaseInfoWidget::sltOpenBirthInfoWidget()
{
	if (m_birthInfoWidget) {
		auto isShow = m_birthInfoWidget->isVisible();
		if (!isShow) {
			int posX = 80;
			int posY = 166;
			m_birthInfoWidget->move(mapToGlobal(QPoint(posX, posY)));
			ui.label_birthIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_sq.png);");
		}
		else {
			ui.label_birthIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_zk.png);");
		}
		m_birthInfoWidget->setVisible(!isShow);
	}
}

void BaseInfoWidget::sltEmotioneFocusState(bool state)
{
	if (!state) {
		ui.label_emotionIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_zk.png);");
		if (m_emotionWidget) {
			m_emotionWidget->setVisible(false);
		}
	}
}

void BaseInfoWidget::sltBirthFocusState(bool state)
{
	if (!state) {
		ui.label_birthIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_zk.png);");
		if (m_birthInfoWidget) {
			m_birthInfoWidget->setVisible(false);
		}
	}
}

void BaseInfoWidget::sltSetEmotion(int emotion)
{
	auto text = EmotionTitles.value((EmotionValue)emotion);
	if (!text.isEmpty()) {
		ui.label_emotion->setText(text);
	}
}

void BaseInfoWidget::sltSetBirthInfo(int year, int month, int day)
{
	if (day < 10) {
		ui.label_birth->setText(QStringLiteral("%1��%2��0%3��").arg(year).arg(month).arg(day));
	}
	else {
		ui.label_birth->setText(QStringLiteral("%1��%2��%3��").arg(year).arg(month).arg(day));
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

#include "LineEdit.h"
#include <QValidator>
#include <QCompleter>
#include <QAbstractItemView>

LineEdit::LineEdit(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	QRegExp regx1("[0-9]+$");
	validator1 = new QRegExpValidator(regx1);
	ui.lineEdit_num->setValidator(validator1);
	ui.lineEdit_num->setAttribute(Qt::WA_InputMethodEnabled, false);
	ui.lineEdit->setAttribute(Qt::WA_InputMethodEnabled, m_IsLimitChInput);

	QRegExp regx2("[a-zA-Z0-9]+$");
	validator2 = new QRegExpValidator(regx2);

	QRegExp regx3("[\u4e00-\u9fa5]");
	validator3 = new QRegExpValidator(regx3);

	//新增一些测试账号
	m_userIdList.append("1001001");
	m_userIdList.append("2001001");
	m_userIdList.append("1101001");
	m_userIdList.append("2010011");
	m_userIdList.append("1101011");
	m_completer = new QCompleter(m_userIdList, this);
	if (m_completer) {
		m_completer->popup()->setStyleSheet("background-color: #152034;\
                                        color: #ffffff;\
                                        border: 1px solid #375C80;\
                                        border-radius: 5px;\
										height: 20px;\
                                        padding: 0px 0px 0px 0px;\
                                        font: 14px \"Arial\";");
		ui.lineEdit->setCompleter(m_completer);
	}

	connect(ui.radioButton1, &QRadioButton::toggled, this, &LineEdit::sltNumLimitToggled);
	connect(ui.radioButton2, &QRadioButton::toggled, this, &LineEdit::sltNumLetterLimitToggled);
	connect(ui.radioButton3, &QRadioButton::toggled, this, &LineEdit::sltNoLimitToggled);
	connect(ui.pushButton_2, &QPushButton::clicked, this, &LineEdit::sltLimitChInput);
	connect(ui.pushButton, &QPushButton::clicked, this, &LineEdit::sltLimitInputLength);
	connect(ui.pushButton_3, &QPushButton::clicked, this, &LineEdit::sltInputIsPassWord);
	connect(ui.lineEdit, &QLineEdit::textEdited, this, &LineEdit::sltTextEdited);
}

LineEdit::~LineEdit()
{
}

void LineEdit::sltNumLetterLimitToggled(bool check)
{
	if (check)
	{
		ui.lineEdit->clear();
		ui.lineEdit->setValidator(validator2);
	}
}

void LineEdit::sltNoLimitToggled(bool check)
{
	if (check)
	{
		ui.lineEdit->clear();
		ui.lineEdit->setValidator(validator3);
	}
}

void LineEdit::sltLimitChInput()
{
	ui.lineEdit->clear();
	m_IsLimitChInput = !m_IsLimitChInput;
	if (m_IsLimitChInput)
		ui.pushButton_2->setText(QStringLiteral("解除屏蔽"));
	else
		ui.pushButton_2->setText(QStringLiteral("解除中文"));
	ui.lineEdit->setAttribute(Qt::WA_InputMethodEnabled, m_IsLimitChInput);
}

void LineEdit::sltLimitInputLength()
{
	ui.lineEdit->clear();
	auto length = ui.lineEdit_num->text().toInt();
	ui.lineEdit->setMaxLength(length);
}

void LineEdit::sltInputIsPassWord()
{
	m_IsPassWordInput = !m_IsPassWordInput;
	if (m_IsPassWordInput)
		ui.lineEdit->setEchoMode(QLineEdit::Password);
	else
		ui.lineEdit->setEchoMode(QLineEdit::Normal);
}

//自动补全
void LineEdit::sltTextEdited(const QString & text)
{
	//遍历账号列表
	if (text.isEmpty()) {
		m_oldLineEditText = text;
		return;
	}
	if (m_oldLineEditText.size() >= text.size()) {
		m_oldLineEditText = text;
		return;
	}
	m_oldLineEditText = text;
	QStringList result = m_userIdList.filter(text);
	if (!result.isEmpty()) {
		QString firstUserId = result[0];
		int index = firstUserId.indexOf(text);
		if (0 == index) {
			ui.lineEdit->setText(firstUserId);
			ui.lineEdit->setSelection(text.size(), firstUserId.size());
		}
	}
}

void LineEdit::sltNumLimitToggled(bool check)
{
	if (check)
	{
		ui.lineEdit->clear();
		ui.lineEdit->setValidator(validator1);
	}
}

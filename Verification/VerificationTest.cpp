#include "VerificationTest.h"

VerificationTest::VerificationTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	ui.widget_verification->setBackgroundStyle(BackgroundStyle::E_DOT);

	connect(ui.pushButton_sure, &QPushButton::clicked, this, &VerificationTest::sltVerficationCodeBtn);
}

VerificationTest::~VerificationTest()
{
}

void VerificationTest::sltVerficationCodeBtn()
{
	auto codes = ui.widget_verification->getVerificationCode().toUpper();
	auto lineEditText = ui.lineEdit->text().toUpper();
	if (codes == lineEditText)
		ui.label->setText(QStringLiteral("验证码输入正确"));
	else
		ui.label->setText(QStringLiteral("验证码输入错误"));
}

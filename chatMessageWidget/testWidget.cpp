#include "testWidget.h"

testWidget::testWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent), dataNum(0)
{
	setFixedSize(200, 200);
	m_test = new QPushButton(this);
	m_test->setGeometry(70, 70, 60, 60);
	m_test->setText("test");

	for (int i = 0; i < 5; i++)
	{
		userDataInfo _pInfo;
		_pInfo.nickName = QStringLiteral("美女%1").arg(i + 1);
		_pInfo.logoPath = QStringLiteral(":/qrc/image/%1.jpg").arg(i + 1);
		_pInfo.text = QStringLiteral("美女机器人%1号").arg(i + 1);
		_pInfo.userId = QStringLiteral("000%1").arg(i + 1);
		m_userDataInfo.insert(i, _pInfo);
	}

	connect(m_test, &QPushButton::clicked, this, [=]() {
		if (dataNum > 4) {
			return;
		}
		m_chat.openChatWidget(m_userDataInfo[dataNum]);
		dataNum++;
		m_chat.show();
	});

	connect(&m_chat, &chatMessageWidget::sigCloseSignal, this, [=]() {
		dataNum = 0;
	});
}

testWidget::~testWidget()
{
}

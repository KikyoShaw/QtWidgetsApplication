#include "userInfoWidget.h"
#include "gameInfoWidget.h"

#define MAXNUM 40

UserInfoWidget::UserInfoWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent)
{
	ui.setupUi(this);

	initGameWidget();

	//打开游戏下拉框
	connect(ui.label_game, &vLabel::sigRightMouseClicked, this, &UserInfoWidget::sltOpenGameInfoWidget);
	connect(ui.label_game, &vLabel::sigFocusState, this, &UserInfoWidget::sltGameFocusState);

	//限制个性签名字数
	connect(ui.textEdit_slogn, &QTextEdit::textChanged, this, &UserInfoWidget::sltSetMaxLengthAndGetLength);
}

UserInfoWidget::~UserInfoWidget()
{
}

void UserInfoWidget::clearUI()
{
	ui.label_game->clearFocus();
	ui.textEdit_slogn->clearFocus();
}

void UserInfoWidget::initGameWidget()
{
	m_gameInfoWidget = new GameInfoWidget(this);
	if (m_gameInfoWidget) {
		m_gameInfoWidget->setVisible(false);
	}
}

void UserInfoWidget::sltOpenGameInfoWidget()
{
	if (m_gameInfoWidget) {
		auto isShow = m_gameInfoWidget->isVisible();
		if (!isShow) {
			int posX = 104;
			int posY = 157;
			m_gameInfoWidget->move(mapToGlobal(QPoint(posX, posY)));
			ui.label_gameIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_sq.png);");
		}
		else {
			ui.label_gameIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_zk.png);");
		}
		m_gameInfoWidget->setVisible(!isShow);
	}
}

void UserInfoWidget::sltGameFocusState(bool state)
{
	if (!state) {
		ui.label_gameIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_zk.png);");
		if (m_gameInfoWidget) {
			m_gameInfoWidget->setVisible(false);
		}
	}
}

void UserInfoWidget::sltSetMaxLengthAndGetLength()
{
	auto textContent = ui.textEdit_slogn->toPlainText();
	auto textNum = textContent.count();
	ui.label_textNum->setText(QString("%1/%2").arg(textNum).arg(MAXNUM));
	if (textNum > MAXNUM) {
		auto position = ui.textEdit_slogn->textCursor().position();
		QTextCursor textCursor = ui.textEdit_slogn->textCursor();
		textContent.remove(position - (textNum - MAXNUM), textNum - MAXNUM);
		ui.textEdit_slogn->setText(textContent);
		textCursor.setPosition(position - (textNum - MAXNUM));
		ui.textEdit_slogn->setTextCursor(textCursor);
	}
}

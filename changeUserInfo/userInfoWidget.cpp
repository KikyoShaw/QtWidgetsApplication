#include "userInfoWidget.h"
#include "gameInfoWidget.h"

#define MAXNUM 40

UserInfoWidget::UserInfoWidget(QWidget *parent /* = Q_NULLPTR */)
	:QWidget(parent), m_gameList(QStringList())
{
	ui.setupUi(this);

	ui.label_gameIcon->setAttribute(Qt::WA_TransparentForMouseEvents, true);
	ui.label_game->setFocusSignals(false);

	//禁止签名框右键功能
	ui.textEdit_slogn->setContextMenuPolicy(Qt::NoContextMenu);

	initGameWidget();

	//打开游戏下拉框
	connect(ui.label_game, &vLabel::sigRightMouseClicked, this, &UserInfoWidget::sltOpenGameInfoWidget);

	//签名框
	connect(ui.textEdit_slogn, &vTextEdits::sigFocusState, this, &UserInfoWidget::sltSlognFocusState);

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
	if (m_gameInfoWidget) {
		m_gameInfoWidget->setVisible(false);
	}
}

void UserInfoWidget::initGameWidget()
{
	m_gameInfoWidget = new GameInfoWidget(this);
	if (m_gameInfoWidget) {
		m_gameInfoWidget->setVisible(false);
		connect(m_gameInfoWidget, &GameInfoWidget::sigChangeParentUI, this, &UserInfoWidget::sltChangeGameLabel);
		connect(m_gameInfoWidget, &GameInfoWidget::sigSelectGameName, this, &UserInfoWidget::sltSetSelectGameName);
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

void UserInfoWidget::sltChangeGameLabel(bool state)
{
	if (state) {
		ui.label_game->setStyleSheet("color: rgba(255,255,255,1);background-color:transparent;border: 1px solid #01EEC3;");
	}
	else {
		ui.label_game->setStyleSheet("color: rgba(255,255,255,0.5);");
		ui.label_gameIcon->setStyleSheet("border-image: url(:/changeUserInfo/qrc/icon_zk.png);");
	}
}

void UserInfoWidget::sltSlognFocusState(bool state)
{
	if (state) {
		ui.widget_textEdit->setStyleSheet("#widget_textEdit{background-color:transparent;border: 1px solid #01EEC3;border-radius: 6px;}");
		if (m_gameInfoWidget) {
			m_gameInfoWidget->setVisible(false);
		}
	}
	else {
		ui.widget_textEdit->setStyleSheet("#widget_textEdit{background: rgba(255,255,255,0.1);border-radius: 6px;}");
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

void UserInfoWidget::sltSetSelectGameName(const QString & name, bool toggled)
{
	if (toggled) {
		if (!m_gameList.contains(name)) {
			m_gameList.append(name);
		}
	}
	else {
		if (m_gameList.contains(name)) {
			m_gameList.removeOne(name);
		}
	}
	QString gameText = QStringLiteral("未设置");
	if (!m_gameList.isEmpty()) {
		gameText = m_gameList.join(QStringLiteral("、"));
	}
	ui.label_game->setText(gameText);
	ui.label_gameNum->setText(QString("%1/3").arg(QString::number(m_gameList.size())));
}
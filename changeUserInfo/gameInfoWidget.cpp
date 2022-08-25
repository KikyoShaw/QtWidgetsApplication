#include "gameInfoWidget.h"
#include "likeGameConfig.h"
#include <QDebug>
#include <QScrollBar>
#include "vGameButton.h"
#include <QFile>

const char* Property_GameName = "gameName";

GameInfoWidget::GameInfoWidget(QWidget *parent)
	:QWidget(parent), m_gameList(QStringList())
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	vLikeGameMgr.loadLikeGameListTable();

	initGameList();

	//滚动条样式
	QString QSS1path = ":/qss/qrc/qss/whiteScrollbar.qss";
	QFile QSS1(QSS1path);
	if (QSS1.open(QIODevice::ReadOnly)) {
	QString strStyle = QSS1.readAll();
		ui.scrollArea_game->verticalScrollBar()->setStyleSheet(strStyle);
		ui.scrollArea_search->verticalScrollBar()->setStyleSheet(strStyle);
	}

	connect(&vLikeGameMgr, &VLikeGameMgr::sigUpdateFinished, this, &GameInfoWidget::initGameList);

	//搜索
	connect(ui.lineEdit_search, &QLineEdit::textChanged, this, &GameInfoWidget::sltGameSearch);
}

GameInfoWidget::~GameInfoWidget()
{
}

void GameInfoWidget::initGameCheck(const QString & text)
{
	for (auto & button : m_gameButtonVec) {
		auto buttonText = button->property(Property_GameName).toString();
		if (buttonText == text) {
			button->setChecked(true);
		}
	}
}

void GameInfoWidget::showWidget()
{
	ui.stackedWidget->setCurrentWidget(ui.page_gameList);
	this->show();
}

void GameInfoWidget::clearData()
{
	m_gameButtonVec.clear();
	m_objectHandler.clear();
}

void GameInfoWidget::initGameList()
{
	//数据清理
	clearData();
	//获取首字母容器
	auto firstWordList = vLikeGameMgr.getFirstWord();
	for (auto &each : firstWordList) {
		addFirstWordObject(each);
		auto gameList = vLikeGameMgr.getGameListByFirstWord(each);
		for (auto &game : gameList) {
			addGameObject(game);
		}
	}
}

void GameInfoWidget::addFirstWordObject(const QString & text)
{
	QLabel *label = new QLabel();
	if (label) {
		label->setFixedHeight(36);
		label->setText(text.toUpper());
		m_objectHandler.add(label);
		ui.verticalLayout_game->addWidget(label);
	}
}

void GameInfoWidget::addGameObject(const QString & text)
{
	GameButton *button = new GameButton();
	if (button) {
		button->setContent(text);
		button->setProperty(Property_GameName, text);
		connect(button, &GameButton::sigToggled, this, &GameInfoWidget::sltGameButtonToggled);
		m_objectHandler.add(button);
		m_gameButtonVec.append(button);
		ui.verticalLayout_game->addWidget(button);
	}
}

void GameInfoWidget::addFirstWordResult(const QString& text)
{
	QLabel *label = new QLabel();
	if (label) {
		label->setFixedHeight(36);
		label->setText(text.toUpper());
		ui.verticalLayout_search->addWidget(label);
		m_searchObjectHandler.add(label);
	}
}

void GameInfoWidget::addGameNameResult(const QString& text)
{
	GameButton *button = new GameButton();
	if (button) {
		button->setContent(text);
		button->setProperty(Property_GameName, text);
		connect(button, &GameButton::sigToggled, this, &GameInfoWidget::sltGameButtonToggled);
		m_searchObjectHandler.add(button);
		ui.verticalLayout_search->addWidget(button);
	}
}

void GameInfoWidget::sltGameSearch(const QString & text)
{
	m_searchObjectHandler.clear();
	if (text.isEmpty()) {
		ui.stackedWidget->setCurrentWidget(ui.page_gameList);
		return;
	}
	ui.stackedWidget->setCurrentWidget(ui.page_search);
	//先采用字母匹配
	auto firstWordList = vLikeGameMgr.getFirstWord();
	if (firstWordList.contains(text)) {
		addFirstWordResult(text);
		auto nameList = vLikeGameMgr.getGameListByFirstWord(text);
		for (auto &game : nameList) {
			addGameNameResult(game);
		}
	}
	else { //最后采用游戏文字匹配
		auto gameList = vLikeGameMgr.getAllGameList();
		QMap<QString, QStringList> gameResultMap;
		for (auto & name : gameList) {
			if (name.contains(text)) {
				QStringList gameResult = QStringList();
				auto firstWord = vLikeGameMgr.getFirstWordByName(name);
				if (gameResultMap.contains(firstWord)) {
					gameResult = gameResultMap.value(firstWord);
				}
				gameResult.append(name);
				gameResultMap.insert(firstWord, gameResult);
			}
		}
		//填充
		auto keys = gameResultMap.keys();
		for (auto &each : keys) {
			addFirstWordResult(each);
			auto values = gameResultMap.value(each);
			for (auto &value : values) {
				addGameNameResult(value);
			}
		}
	}
}

void GameInfoWidget::sltGameButtonToggled(const QString& name, bool toggled, GameButton *obj)
{
	if (Q_NULLPTR == obj)
		return;

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

	auto _size = m_gameList.size();
	if (3 < _size) {
		obj->setChecked(false);
		return;
	}
	emit sigSelectGameName(name, toggled);
}

void GameInfoWidget::showEvent(QShowEvent * event)
{
	emit sigChangeParentUI(true);
	QWidget::showEvent(event);
}

void GameInfoWidget::closeEvent(QCloseEvent * event)
{
	emit sigChangeParentUI(false);
	QWidget::closeEvent(event);
}

void GameInfoWidget::hideEvent(QHideEvent * event)
{
	emit sigChangeParentUI(false);
	QWidget::hideEvent(event);
}

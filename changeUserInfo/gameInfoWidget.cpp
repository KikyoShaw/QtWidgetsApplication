#include "gameInfoWidget.h"
#include "pinyin.h"
#include <QDebug>

GameInfoWidget::GameInfoWidget(QWidget *parent)
	:QWidget(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);

	//test
	QString text = QStringLiteral("明日之后");
	QString firstWord = firstPinYinLetter(text);
	qInfo() << firstWord.toUpper();
}

GameInfoWidget::~GameInfoWidget()
{
}

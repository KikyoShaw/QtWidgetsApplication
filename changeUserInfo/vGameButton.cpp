#include "vGameButton.h"
#include <QMouseEvent>

GameButton::GameButton(QWidget *parent /* = Q_NULLPTR */)
	:QCheckBox(parent), m_gameName(QString())
{
	ui.setupUi(this);

	connect(this, &QCheckBox::toggled, this, [=](bool check) {
		emit sigToggled(m_gameName, check, this);
	});
}

GameButton::~GameButton()
{

}

void GameButton::setContent(const QString & text)
{
	m_gameName = text;
	ui.label_text->setText(text);
}

void GameButton::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		this->click();
	}
	QWidget::mousePressEvent(event);
}

#include "moveWidgetTest.h"
#include "moveWidget.h"

moveWidgetTest::moveWidgetTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	m_moveWidget = new MoveWidget(this);
	m_moveWidget->move(340, 140);
	m_moveWidget->hide();

	connect(ui.pushButton, &QPushButton::clicked, this, [=]() {
		m_moveWidget->toStartMoveTimer(false, 0, 100);
		m_moveWidget->show();
	});

}

moveWidgetTest::~moveWidgetTest()
{
}

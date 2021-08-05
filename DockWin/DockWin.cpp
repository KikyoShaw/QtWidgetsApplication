#include "DockWin.h"
#include <QDockWidget>
#include <QCalendarWidget>
#include <QTextEdit>

DockWin::DockWin(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	QDockWidget *dockWdt_1 = new QDockWidget(QStringLiteral("日历"));
	dockWdt_1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);//可移动、可浮动、不可关闭
	dockWdt_1->setWidget(new QCalendarWidget());//在停靠窗体里添加一个日历控件（任何控件都可以）
	dockWdt_1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);//允许停靠在左、右、上
	this->addDockWidget(Qt::RightDockWidgetArea, dockWdt_1);//把Dock添加到MainWindow

	QDockWidget *dockWdt_2 = new QDockWidget(QStringLiteral("文本编辑器"));
	dockWdt_2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);//可移动、可浮动、不可关闭
	dockWdt_2->setWidget(new QTextEdit());//在停靠窗体里添加一个编辑器控件（任何控件都可以）
	dockWdt_2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);//允许停靠在左、右、上
	this->addDockWidget(Qt::RightDockWidgetArea, dockWdt_2);//把Dock添加到MainWindow

	this->setDockNestingEnabled(false);//禁止Dock嵌套
	tabifyDockWidget(dockWdt_1, dockWdt_2);//把两个Dock合并到一个Tab控件里面 ,叠加三个以上QDockWidget时，应该要注意顺序
}

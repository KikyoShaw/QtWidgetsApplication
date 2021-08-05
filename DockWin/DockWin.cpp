#include "DockWin.h"
#include <QDockWidget>
#include <QCalendarWidget>
#include <QTextEdit>

DockWin::DockWin(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	QDockWidget *dockWdt_1 = new QDockWidget(QStringLiteral("����"));
	dockWdt_1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);//���ƶ����ɸ��������ɹر�
	dockWdt_1->setWidget(new QCalendarWidget());//��ͣ�����������һ�������ؼ����κοؼ������ԣ�
	dockWdt_1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);//����ͣ�������ҡ���
	this->addDockWidget(Qt::RightDockWidgetArea, dockWdt_1);//��Dock��ӵ�MainWindow

	QDockWidget *dockWdt_2 = new QDockWidget(QStringLiteral("�ı��༭��"));
	dockWdt_2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);//���ƶ����ɸ��������ɹر�
	dockWdt_2->setWidget(new QTextEdit());//��ͣ�����������һ���༭���ؼ����κοؼ������ԣ�
	dockWdt_2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);//����ͣ�������ҡ���
	this->addDockWidget(Qt::RightDockWidgetArea, dockWdt_2);//��Dock��ӵ�MainWindow

	this->setDockNestingEnabled(false);//��ֹDockǶ��
	tabifyDockWidget(dockWdt_1, dockWdt_2);//������Dock�ϲ���һ��Tab�ؼ����� ,������������QDockWidgetʱ��Ӧ��Ҫע��˳��
}

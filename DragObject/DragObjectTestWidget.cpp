#include "DragObjectTestWidget.h"
#include "DragObject.h"

DragObjectTestWidget::DragObjectTestWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	DragObject *d1 = new DragObject(this);
	if (d1) {
		d1->setWidget(ui.widget);
	}

	DragObject *d2 = new DragObject(this);
	if (d2) {
		d2->setWidget(ui.progressBar);
	}

	DragObject *d3= new DragObject(this);
	if (d3) {
		d3->setWidget(ui.pushButton);
	}

	DragObject *d4 = new DragObject(this);
	if (d4) {
		d4->setWidget(ui.lcdNumber);
	}

}

DragObjectTestWidget::~DragObjectTestWidget()
{
}

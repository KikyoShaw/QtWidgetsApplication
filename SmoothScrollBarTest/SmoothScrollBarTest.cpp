#include "SmoothScrollBarTest.h"
#include "SmoothScrollArea.h"

SmoothScrollBarTest::SmoothScrollBarTest(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	SmoothScrollArea *area = new SmoothScrollArea();
	area->InitSmoothScrollArea();
	setCentralWidget(area);
}

SmoothScrollBarTest::~SmoothScrollBarTest()
{}

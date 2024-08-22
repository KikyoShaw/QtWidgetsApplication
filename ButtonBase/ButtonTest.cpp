#include "ButtonTest.h"

ButtonBase::ButtonBase(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.toolButtonV2->setSwitchButtonType(emSwitchButtonType::V2);
    ui.toolButtonV3->setSwitchButtonType(emSwitchButtonType::V3);
}

ButtonBase::~ButtonBase()
{}

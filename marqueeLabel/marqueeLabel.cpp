#include "marqueeLabel.h"

marqueeLabel::marqueeLabel(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	ui.label->setText(QStringLiteral("����һǧ�꣬����һǧ�꣬��Ҷ���������������������˰��������˰����κ���ǰ���κ�"));

}

#include "marqueeLabel.h"

marqueeLabel::marqueeLabel(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	ui.label->setText(QStringLiteral("花开一千年，花落一千年，花叶生生相错，世世永不相见。彼岸花开开彼岸，奈何桥前可奈何"));

}

#include "pinyinTest.h"
#include "pinyin.h"

pinyinTest::pinyinTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	connect(ui.lineEdit, &QLineEdit::textChanged, this, &pinyinTest::sltTextChanged);
}

pinyinTest::~pinyinTest()
{
}

void pinyinTest::sltTextChanged(const QString & text)
{
	auto firstWord = getFirstPinYinLetter(text);
	ui.label_1->setText(QStringLiteral("Ê××ÖÄ¸: %1").arg(firstWord));

	auto word = getPinYinLetter(text);
	ui.label_2->setText(QStringLiteral("×ÖÄ¸: %1").arg(word));
}

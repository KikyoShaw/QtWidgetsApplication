#include "ComboBoxTest.h"
#include <QLineEdit>
#include <QAbstractItemView>
#include <QFile>
#include <QScrollBar>
#include "accountitem.h"

ComboBoxTest::ComboBoxTest(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

   /* setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);*/

    //设置QComboBox可编辑
    ui.comboBox_userName->setEditable(true);

    //设置QComboBox文案居中
   /* ui.comboBox_userName->lineEdit()->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < ui.comboBox_userName->count(); ++i) {
        ui.comboBox_userName->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }*/

    //下拉框设置圆角样式后，边角会存在阴影问题，这是Popup问题导致的，解决办法可以通过解决Popup来解决，也可以通过设置QComboBox本身属性。
    ui.comboBox_userName->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    ui.comboBox_userName->view()->window()->setAttribute(Qt::WA_TranslucentBackground);

    ui.comboBox_userName->lineEdit()->setPlaceholderText(QStringLiteral("请输入账号"));

    //限制
    QValidator* validator = new QRegExpValidator(QRegExp("[0-9]+$"));
    ui.comboBox_userName->lineEdit()->setValidator(validator);

	// 设置代理;
	QListWidget listWidget;
	m_AccountList = new QListWidget();
	ui.comboBox_userName->setModel(m_AccountList->model());
	ui.comboBox_userName->setView(m_AccountList);
	ui.comboBox_userName->setMaxVisibleItems(3);
	ui.comboBox_userName->view()->window()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
	ui.comboBox_userName->view()->window()->setAttribute(Qt::WA_TranslucentBackground);
	QString QSS1path = ":/qss/qrc/qss/listWidget.qss";
	QFile QSS1(QSS1path);
	if (QSS1.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS1.readAll();
		m_AccountList->setStyleSheet(strStyle);
	}
	m_AccountList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//滚动条样式
	QString QSS2path = ":/qss/qrc/qss/whiteScrollbar.qss";
	QFile QSS2(QSS2path);
	if (QSS2.open(QIODevice::ReadOnly)) {
		QString strStyle = QSS2.readAll();
		m_AccountList->verticalScrollBar()->setStyleSheet(strStyle);
	}
	m_AccountList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	//构造数据
	ui.comboBox_userName->setEditText("100101");
	for (int i = 0; i < 13; i++)
	{
		AccountItem* account_item = new AccountItem();
		userInfo info;
		info.userId = "10010" + QString::number(i+1);
		info.nickName = QStringLiteral("动漫人物%1号").arg(i+1);
		info.path = QStringLiteral(":/ComboBoxTest/qrc/th-c%1.png").arg(i+1);
		account_item->setAccountInfo(i, info);
		connect(account_item, &AccountItem::signalShowAccountInfo, this, &ComboBoxTest::sltShowAccountInfo);
		connect(account_item, &AccountItem::signalRemoveAccount, this, &ComboBoxTest::sltRemoveAccount);
		QListWidgetItem* list_item = new QListWidgetItem(m_AccountList);
		m_AccountList->addItem(list_item);
		m_AccountList->setItemWidget(list_item, account_item);
	}

	/*connect(ui.comboBox_userName, &vComboBox::sigPopup, this, [=]() {
		m_AccountList->setCurrentRow(3);
	});*/

	//connect(ui.comboBox_userName->view(), SIGNAL(viewportEntered()), this, SLOT(sltComboBoxHide()));
}

ComboBoxTest::~ComboBoxTest()
{
}

void ComboBoxTest::sltShowAccountInfo(int index, const QString& accountUserId)
{
	ui.comboBox_userName->setEditText(accountUserId);
	m_AccountList->setCurrentRow(index);
	ui.comboBox_userName->hidePopup();
}

void ComboBoxTest::sltRemoveAccount(int index, const QString& accountUserId)
{
	for (int row = 0; row < m_AccountList->count(); row++)
	{
		AccountItem* itemWidget = (AccountItem*)m_AccountList->itemWidget(m_AccountList->item(row));
		if (itemWidget != NULL && itemWidget->getItemWidgetIndex() == index)
		{
			m_AccountList->takeItem(row);
			itemWidget->deleteLater();
		}
	}
	ui.comboBox_userName->lineEdit()->clear();
	ui.comboBox_userName->hidePopup();
}

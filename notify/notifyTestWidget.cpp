#include "notifyTestWidget.h"
#include "notifymanager.h"

const QStringList notifies = {
    QStringLiteral("ϸ����ǰ��Ҷ������������������ˮ�����������˴������㱻����ʪ���ģ��Ƿ����ɡ�"),
    QStringLiteral("վ�ں���������У������ѩ�����׷�����ذ�������������ĳ��С�������ȥ�˵��Ƿ���ֿ���޼����壬���̲�ס��Ȼ���¡�"),
    QStringLiteral("�����������ߣ���Ӳ���ɡ�������Լ�����գ����Ӳ����ꡣ"),
    QStringLiteral("�Ƕ����£����۴Ӻ��ֽǶȶ��㣬�㶼������ȱ������ȱ�ٵĲ��֣�Ҳ���ѱ���������Ļ���������"),
    QStringLiteral("�������£������о�ȫ�ǰ�쵵Ĺ�Ӱ������������У������Ķ��������ѽ���Զȥ��"),
    QStringLiteral("�����ǳ����ڼ����е��Σ���������Ψһ�����ļ��䣬����Ҷ�ϵ�ˮ�飬��˼�����Ρ�"),
    QStringLiteral("��������������������һ�����ֵ���裬�����ļʵĹ¶�������εȻ�ɱ������㣬�����������������������"),
    QStringLiteral("�ư��˼�һ��������ʱ����ȫ��������������˵���ɫ�����ǣ������ˣ�ҹ���ˣ���Ҳ���ˡ�"),
    QStringLiteral("�ڽ����ӣ��Ͱ��׵غ�����Ϊ���������죻û���ç����΢Ц��ͬ������Ⱦ�̴�ء���������������ƴ������������껪��"),
    QStringLiteral("ͯ�����һ����׷����Ц�����ӣ�����������ˮ�����ҵ���������;�����ʡ�"),
    QStringLiteral("���ñ����ǻ��ĽŲ��������������õ����룬����û�в�ɢ����ϯ��Ҳ�����˼����ֻ���������ʲ����档"),
    QStringLiteral("һ�������ɪ�ĺ�ҹ�����������¶��ǻ����¶�����������Ҹ����������������ķ羰֮���ұ㲻�ٱ�ԹΪʲô����������̫����ź������ٸ�̾�����ľ�ͷ�������̡�")
};

notifyTestWidget::notifyTestWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    manager = new NotifyManager(this);
    manager->setMaxCount(5);
    int time = ui.lineEdit_num->text().toInt() * 1000;
    manager->setDisplayTime(time);
    manager->setNotifyWndSize(360, 120);
    srand(QDateTime::currentMSecsSinceEpoch());
    connect(ui.pushButton_one, &QPushButton::clicked, this, [this]() {
        int pos = rand() % 12;
        QString text = notifies[pos];
        QString url = QStringLiteral(":/notify/image/%1.jpg").arg(pos);
        manager->notify(QStringLiteral("����Ϣ%1").arg(1), text, url);
        });
    connect(ui.pushButton_mul, &QPushButton::clicked, this, [this]() {
        int count = ui.lineEdit_count->text().toInt();
        for (int i = 0; i < count; i++) 
        {
            int pos = rand() % 12;
            QString text = notifies[pos];
            QString url = QStringLiteral(":/notify/image/%1.jpg").arg(pos);
            manager->notify(QStringLiteral("����Ϣ%1").arg(i), text, url);
        }
        });

}

notifyTestWidget::~notifyTestWidget()
{}

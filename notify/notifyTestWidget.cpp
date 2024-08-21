#include "notifyTestWidget.h"
#include "notifymanager.h"

const QStringList notifies = {
    QStringLiteral("细数门前落叶，倾听窗外雨声，涉水而过的声音此次响起，你被雨淋湿的心，是否依旧。"),
    QStringLiteral("站在寒冬的冷风中，漫天的雪花正纷纷扬扬地包裹着这座寒冷的城市。想着逝去了的那份真挚的无价情义，我忍不住怆然泪下。"),
    QStringLiteral("你在雨中行走，你从不打伞；你有自己的天空，它从不下雨。"),
    QStringLiteral("那段岁月，无论从何种角度读你，你都完美无缺，你所缺少的部分，也早已被我用想象的画笔填满。"),
    QStringLiteral("回首往事，日子中竟全是斑斓的光影，记忆的屏障中，曾经心动的声音已渐渐远去。"),
    QStringLiteral("往事是尘封在记忆中的梦，而你是我唯一鲜明的记忆，那绿叶上的水珠，是思念的泪滴。"),
    QStringLiteral("相信优美的生命，就是一曲无字的挽歌，漫过心际的孤独，早已蔚然成冰，而你，是这个季节最美丽的音符。"),
    QStringLiteral("酒般的思念，一饮就醉，醉时就用全部的热情读这忧伤的月色。于是，月醉了，夜醉了，我也醉了。"),
    QStringLiteral("融进银河，就安谧地和明月为伴照亮长天；没入草莽，就微笑着同清风合力染绿大地。这样，才算得上善待生命，不负年华。"),
    QStringLiteral("童年的那一个个追逐嬉笑的日子，有如行云流水般在我的生命的旅途中流淌。"),
    QStringLiteral("别让别人徘徊的脚步踩碎你明天美好的梦想，天下没有不散的宴席。也许这人间真的只有朦朦胧胧才是真。"),
    QStringLiteral("一个秋风萧瑟的寒夜，当天边那弯孤独徘徊的月儿轻轻告诉了我该如何描绘心中最美的风景之后，我便不再抱怨为什么人生总是有太多的遗憾，不再感叹生命的尽头总是轻烟。")
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
        manager->notify(QStringLiteral("新消息%1").arg(1), text, url);
        });
    connect(ui.pushButton_mul, &QPushButton::clicked, this, [this]() {
        int count = ui.lineEdit_count->text().toInt();
        for (int i = 0; i < count; i++) 
        {
            int pos = rand() % 12;
            QString text = notifies[pos];
            QString url = QStringLiteral(":/notify/image/%1.jpg").arg(pos);
            manager->notify(QStringLiteral("新消息%1").arg(i), text, url);
        }
        });

}

notifyTestWidget::~notifyTestWidget()
{}

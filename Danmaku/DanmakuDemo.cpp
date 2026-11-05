#include "DanmakuDemo.h"
#include "DanmakuWidget.h"

#include <QCheckBox>
#include <QColor>
#include <QComboBox>
#include <QEvent>
#include <QFrame>
#include <QHideEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPushButton>
#include <QRandomGenerator>
#include <QRadialGradient>
#include <QScrollArea>
#include <QShowEvent>
#include <QSignalBlocker>
#include <QSlider>
#include <QSpinBox>
#include <QStackedLayout>
#include <QStringList>
#include <QTimer>
#include <QVariant>
#include <QVBoxLayout>
#include <QtMath>

namespace {

class Backdrop : public QWidget
{
public:
    explicit Backdrop(QWidget *parent) : QWidget(parent)
    {
        setAttribute(Qt::WA_OpaquePaintEvent);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        const QSize logicalSize = size();
        const qreal dpr = devicePixelRatioF();
        if (m_cache.isNull() || m_cachedSize != logicalSize || m_cachedDpr != dpr) {
            m_cache = QPixmap();
            if (!logicalSize.isEmpty()) {
                QPixmap cache(qCeil(logicalSize.width() * dpr),
                              qCeil(logicalSize.height() * dpr));
                if (!cache.isNull()) {
                    cache.setDevicePixelRatio(dpr);
                    // Cover the fractional edge pixels introduced by rounding up.
                    cache.fill(QColor(QStringLiteral("#192D49")));
                    QPainter cachePainter(&cache);
                    if (cachePainter.isActive()) {
                        paintBackdrop(cachePainter);
                        cachePainter.end();
                        m_cache = cache;
                    }
                }
            }
            m_cachedSize = logicalSize;
            m_cachedDpr = dpr;
        }

        QPainter painter(this);
        if (!m_cache.isNull())
            painter.drawPixmap(QPointF(0, 0), m_cache);
        else
            paintBackdrop(painter);
    }

private:
    void paintBackdrop(QPainter &painter) const
    {
        painter.fillRect(rect(), QColor(QStringLiteral("#192D49")));
        if (size().isEmpty())
            return;

        painter.setRenderHint(QPainter::Antialiasing);
        painter.scale(width() / 1000.0, height() / 650.0);
        painter.setPen(Qt::NoPen);

        QLinearGradient sky(0, 0, 0, 650);
        sky.setColorAt(0, QColor(QStringLiteral("#192D49")));
        sky.setColorAt(0.58, QColor(QStringLiteral("#8096A8")));
        sky.setColorAt(1, QColor(QStringLiteral("#EBC5AE")));
        painter.fillRect(QRectF(0, 0, 1000, 650), sky);

        QRadialGradient glow(QPointF(760, 218), 170);
        glow.setColorAt(0, QColor(255, 225, 180, 90));
        glow.setColorAt(1, QColor(255, 225, 180, 0));
        painter.setBrush(glow);
        painter.drawEllipse(QPointF(760, 218), 170, 170);
        painter.setBrush(QColor(QStringLiteral("#FFE4B9")));
        painter.drawEllipse(QPointF(760, 218), 35, 35);

        QPainterPath distant;
        distant.moveTo(0, 422);
        distant.lineTo(0, 331);
        distant.lineTo(106, 245);
        distant.lineTo(180, 302);
        distant.lineTo(306, 172);
        distant.lineTo(408, 278);
        distant.lineTo(496, 222);
        distant.lineTo(640, 348);
        distant.lineTo(824, 281);
        distant.lineTo(1000, 366);
        distant.lineTo(1000, 480);
        distant.closeSubpath();
        painter.fillPath(distant, QColor(QStringLiteral("#708299")));

        QPainterPath middle;
        middle.moveTo(0, 467);
        middle.lineTo(0, 378);
        middle.cubicTo(115, 341, 184, 420, 281, 361);
        middle.lineTo(398, 284);
        middle.lineTo(528, 390);
        middle.lineTo(637, 330);
        middle.lineTo(768, 404);
        middle.lineTo(893, 342);
        middle.lineTo(1000, 389);
        middle.lineTo(1000, 510);
        middle.closeSubpath();
        painter.fillPath(middle, QColor(QStringLiteral("#415D77")));

        QLinearGradient water(0, 452, 0, 650);
        water.setColorAt(0, QColor(QStringLiteral("#6F8C9C")));
        water.setColorAt(1, QColor(QStringLiteral("#233F57")));
        painter.fillRect(QRectF(0, 452, 1000, 198), water);

        painter.setPen(QPen(QColor(225, 231, 229, 55), 1.2));
        for (int i = 0; i < 9; ++i) {
            const qreal y = 474 + i * 18;
            const qreal halfWidth = 20 + i * 9;
            painter.drawLine(QPointF(760 - halfWidth, y), QPointF(760 + halfWidth, y));
        }
        painter.setPen(Qt::NoPen);

        QPainterPath shore;
        shore.moveTo(0, 650);
        shore.lineTo(0, 411);
        shore.cubicTo(130, 434, 146, 487, 280, 509);
        shore.cubicTo(366, 524, 416, 538, 492, 575);
        shore.cubicTo(349, 576, 240, 610, 224, 650);
        shore.closeSubpath();
        painter.fillPath(shore, QColor(QStringLiteral("#1D354A")));

        QPainterPath foreground;
        foreground.moveTo(1000, 487);
        foreground.cubicTo(912, 510, 928, 543, 824, 570);
        foreground.cubicTo(756, 590, 698, 614, 658, 650);
        foreground.lineTo(1000, 650);
        foreground.closeSubpath();
        painter.fillPath(foreground, QColor(QStringLiteral("#172E42")));
    }

    QPixmap m_cache;
    QSize m_cachedSize;
    qreal m_cachedDpr = 0.0;
};

const QStringList &sampleTexts()
{
    static const QStringList texts = {
        QStringLiteral("山色正好，弹幕出发！"),
        QStringLiteral("你好，弹幕世界"),
        QStringLiteral("这片晚霞真好看"),
        QStringLiteral("长一点的文字也会沿着通道慢慢飘过，留住眼前这一刻。"),
        QStringLiteral("一起看山，一起聊天"),
        QStringLiteral("来了来了"),
        QStringLiteral("试试把字号调大一点"),
        QStringLiteral("每一条弹幕都有自己的颜色"),
        QStringLiteral("今天也是元气满满的一天"),
        QStringLiteral("让文字跟着风走"),
        QStringLiteral("前方风景，请慢慢欣赏"),
        QStringLiteral("短句也精彩"),
        QStringLiteral("暂停一下，看看正在飘过的文字"),
        QStringLiteral("换个速度，换种心情"),
        QStringLiteral("透明度可以随时调整，山景始终清晰"),
        QStringLiteral("湖面有光，远处有山"),
        QStringLiteral("在下方输入一句话，按 Enter 就能加入弹幕"),
        QStringLiteral("这是一条用来体验长文本滚动效果的中文弹幕，愿你今天也有好心情。"),
        QStringLiteral("刚刚好"),
        QStringLiteral("把喜欢的话留在这里")
    };
    return texts;
}

QColor randomTextColor()
{
    static const QColor colors[] = {
        QColor(QStringLiteral("#FFFFFF")),
        QColor(QStringLiteral("#FF8B8B")),
        QColor(QStringLiteral("#FFE08A")),
        QColor(QStringLiteral("#94E6BA")),
        QColor(QStringLiteral("#9BD1FF"))
    };
    return colors[QRandomGenerator::global()->bounded(5)];
}

} // namespace

DanmakuDemo::DanmakuDemo(QWidget *parent) : QWidget(parent)
{
    setObjectName(QStringLiteral("DanmakuDemo"));
    setWindowTitle(QStringLiteral("Danmaku · 弹幕飘屏"));
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(860, 560);
    resize(1120, 740);
    setStyleSheet(QStringLiteral(
        "QWidget#DanmakuDemo { background: #F1F5F9; }"
        "QLabel { color: #334155; font-size: 13px; }"
        "QLabel#DemoTitle { color: #0F172A; font-size: 23px; font-weight: 600; }"
        "QLabel#DemoDescription, QLabel#SceneCaption, QLabel#SettingHint { color: #64748B; font-size: 12px; }"
        "QFrame#SceneCard, QWidget#ControlPanel, QFrame#InputCard { background: #FFFFFF;"
        " border: 1px solid #E2E8F0; border-radius: 9px; }"
        "QLabel#PanelTitle { color: #0F172A; font-size: 17px; font-weight: 600; }"
        "QLabel#SettingTitle { color: #334155; font-size: 14px; font-weight: 600; }"
        "QFrame#FontSetting { background: #EFF6FF; border: 1px solid #BFDBFE; border-radius: 7px; }"
        "QSpinBox, QComboBox, QLineEdit { background: #FFFFFF; color: #0F172A;"
        " border: 1px solid #CBD5E1; border-radius: 5px; min-height: 30px; padding: 2px 7px; font-size: 13px; }"
        "QSpinBox#FontSizeSpin { color: #1D4ED8; font-size: 18px; font-weight: 600; }"
        "QSpinBox:focus, QComboBox:focus, QLineEdit:focus { border-color: #3B82F6; }"
        "QPushButton { color: #334155; background: #FFFFFF; border: 1px solid #CBD5E1;"
        " border-radius: 5px; padding: 8px 15px; font-size: 13px; }"
        "QPushButton:hover { background: #F1F5F9; border-color: #94A3B8; }"
        "QPushButton:checked { color: #1D4ED8; background: #EFF6FF; border-color: #93C5FD; }"
        "QPushButton#SendButton { color: #FFFFFF; background: #2563EB; border-color: #2563EB; }"
        "QPushButton#SendButton:hover { background: #1D4ED8; }"
        "QCheckBox { color: #334155; font-size: 13px; spacing: 7px; }"
        "QSlider::groove:horizontal { height: 5px; background: #E2E8F0; border-radius: 2px; }"
        "QSlider::sub-page:horizontal { background: #60A5FA; border-radius: 2px; }"
        "QSlider::handle:horizontal { width: 14px; margin: -5px 0; border-radius: 7px;"
        " background: #2563EB; }"
        "QScrollArea#ControlScroll { border: none; background: transparent; }"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(18, 16, 18, 16);
    layout->setSpacing(10);
    auto *title = new QLabel(QStringLiteral("弹幕飘屏"), this);
    title->setObjectName(QStringLiteral("DemoTitle"));
    layout->addWidget(title);
    auto *description = new QLabel(
        QStringLiteral("本地弹幕演示 · 静态山景背景，体验多通道文字从右向左滚动。"), this);
    description->setObjectName(QStringLiteral("DemoDescription"));
    description->setWordWrap(true);
    layout->addWidget(description);

    auto *content = new QHBoxLayout;
    content->setSpacing(14);
    layout->addLayout(content, 1);

    auto *sceneCard = new QFrame(this);
    sceneCard->setObjectName(QStringLiteral("SceneCard"));
    auto *sceneLayout = new QVBoxLayout(sceneCard);
    sceneLayout->setContentsMargins(8, 8, 8, 10);
    sceneLayout->setSpacing(8);
    auto *scene = new QWidget(sceneCard);
    scene->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    auto *stack = new QStackedLayout(scene);
    stack->setContentsMargins(0, 0, 0, 0);
    stack->setStackingMode(QStackedLayout::StackAll);
    auto *backdrop = new Backdrop(scene);
    m_danmaku = new DanmakuWidget(scene);
    m_danmaku->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_danmaku->setAutoFillBackground(false);
    stack->addWidget(backdrop);
    stack->addWidget(m_danmaku);
    stack->setCurrentWidget(m_danmaku);
    sceneLayout->addWidget(scene, 1);
    auto *caption = new QLabel(
        QStringLiteral("山色之间 · 背景为本地绘制的静态山景，透明度仅影响弹幕文字。"), sceneCard);
    caption->setObjectName(QStringLiteral("SceneCaption"));
    caption->setWordWrap(true);
    sceneLayout->addWidget(caption);
    content->addWidget(sceneCard, 1);

    m_danmaku->setFontSize(24);
    m_danmaku->setLaneCount(6);
    m_danmaku->setOpacity(0.85);
    m_danmaku->setScrollSpeed(120);

    auto *scroll = new QScrollArea(this);
    scroll->setObjectName(QStringLiteral("ControlScroll"));
    scroll->setFixedWidth(286);
    scroll->setWidgetResizable(true);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto *panel = new QWidget(scroll);
    panel->setObjectName(QStringLiteral("ControlPanel"));
    auto *panelLayout = new QVBoxLayout(panel);
    panelLayout->setContentsMargins(14, 15, 14, 15);
    panelLayout->setSpacing(14);
    auto *panelTitle = new QLabel(QStringLiteral("弹幕设置"), panel);
    panelTitle->setObjectName(QStringLiteral("PanelTitle"));
    panelLayout->addWidget(panelTitle);

    auto *fontSetting = new QFrame(panel);
    fontSetting->setObjectName(QStringLiteral("FontSetting"));
    auto *fontLayout = new QVBoxLayout(fontSetting);
    fontLayout->setContentsMargins(10, 10, 10, 10);
    fontLayout->setSpacing(10);
    auto *fontRow = new QHBoxLayout;
    auto *fontLabel = new QLabel(QStringLiteral("字号"), fontSetting);
    fontLabel->setObjectName(QStringLiteral("SettingTitle"));
    m_fontSpin = new QSpinBox(fontSetting);
    m_fontSpin->setObjectName(QStringLiteral("FontSizeSpin"));
    m_fontSpin->setRange(16, 48);
    m_fontSpin->setSuffix(QStringLiteral(" px"));
    m_fontSpin->setFixedWidth(100);
    m_fontSpin->setAccessibleName(QStringLiteral("弹幕字号，16 到 48 像素"));
    fontRow->addWidget(fontLabel, 1);
    fontRow->addWidget(m_fontSpin);
    fontLayout->addLayout(fontRow);
    m_fontSlider = new QSlider(Qt::Horizontal, fontSetting);
    m_fontSlider->setRange(16, 48);
    m_fontSlider->setAccessibleName(QStringLiteral("弹幕字号滑条"));
    fontLayout->addWidget(m_fontSlider);
    panelLayout->addWidget(fontSetting);

    auto *laneRow = new QHBoxLayout;
    auto *laneLabel = new QLabel(QStringLiteral("显示通道"), panel);
    laneLabel->setObjectName(QStringLiteral("SettingTitle"));
    m_laneSpin = new QSpinBox(panel);
    m_laneSpin->setRange(1, 12);
    m_laneSpin->setSuffix(QStringLiteral(" 条"));
    m_laneSpin->setFixedWidth(100);
    m_laneSpin->setAccessibleName(QStringLiteral("请求通道数，1 到 12 条"));
    laneRow->addWidget(laneLabel, 1);
    laneRow->addWidget(m_laneSpin);
    panelLayout->addLayout(laneRow);

    auto *opacityRow = new QHBoxLayout;
    auto *opacityLabel = new QLabel(QStringLiteral("文字透明度"), panel);
    opacityLabel->setObjectName(QStringLiteral("SettingTitle"));
    m_opacityValue = new QLabel(panel);
    m_opacityValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    opacityRow->addWidget(opacityLabel, 1);
    opacityRow->addWidget(m_opacityValue);
    panelLayout->addLayout(opacityRow);
    m_opacitySlider = new QSlider(Qt::Horizontal, panel);
    m_opacitySlider->setRange(0, 100);
    m_opacitySlider->setAccessibleName(QStringLiteral("弹幕文字透明度百分比"));
    panelLayout->addWidget(m_opacitySlider);

    auto *speedRow = new QHBoxLayout;
    auto *speedLabel = new QLabel(QStringLiteral("滚动速度"), panel);
    speedLabel->setObjectName(QStringLiteral("SettingTitle"));
    m_speedSpin = new QSpinBox(panel);
    m_speedSpin->setRange(60, 240);
    m_speedSpin->setSuffix(QStringLiteral(" px/s"));
    m_speedSpin->setFixedWidth(108);
    m_speedSpin->setAccessibleName(QStringLiteral("滚动速度，60 到 240 像素每秒"));
    speedRow->addWidget(speedLabel, 1);
    speedRow->addWidget(m_speedSpin);
    panelLayout->addLayout(speedRow);
    m_speedSlider = new QSlider(Qt::Horizontal, panel);
    m_speedSlider->setRange(60, 240);
    m_speedSlider->setAccessibleName(QStringLiteral("弹幕滚动速度滑条"));
    panelLayout->addWidget(m_speedSlider);

    auto *actions = new QHBoxLayout;
    m_pauseButton = new QPushButton(QStringLiteral("暂停"), panel);
    m_pauseButton->setCheckable(true);
    auto *clearButton = new QPushButton(QStringLiteral("清空"), panel);
    actions->addWidget(m_pauseButton, 1);
    actions->addWidget(clearButton, 1);
    panelLayout->addLayout(actions);
    m_autoSamples = new QCheckBox(QStringLiteral("自动发送样例"), panel);
    m_autoSamples->setChecked(true);
    panelLayout->addWidget(m_autoSamples);
    auto *sampleHint = new QLabel(QStringLiteral("每 1.2 秒尝试发送一条；有排队时等待。"), panel);
    sampleHint->setObjectName(QStringLiteral("SettingHint"));
    sampleHint->setWordWrap(true);
    panelLayout->addWidget(sampleHint);
    m_statistics = new QLabel(panel);
    m_statistics->setWordWrap(true);
    panelLayout->addWidget(m_statistics);
    m_laneHint = new QLabel(panel);
    m_laneHint->setObjectName(QStringLiteral("SettingHint"));
    m_laneHint->setWordWrap(true);
    panelLayout->addWidget(m_laneHint);
    panelLayout->addStretch(1);
    scroll->setWidget(panel);
    content->addWidget(scroll);

    auto *inputCard = new QFrame(this);
    inputCard->setObjectName(QStringLiteral("InputCard"));
    auto *inputLayout = new QHBoxLayout(inputCard);
    inputLayout->setContentsMargins(10, 9, 10, 9);
    inputLayout->setSpacing(10);
    m_input = new QLineEdit(inputCard);
    m_input->setPlaceholderText(QStringLiteral("输入一条弹幕，按 Enter 发送…"));
    m_input->setAccessibleName(QStringLiteral("弹幕内容"));
    inputLayout->addWidget(m_input, 1);
    m_colorCombo = new QComboBox(inputCard);
    m_colorCombo->setAccessibleName(QStringLiteral("弹幕颜色"));
    m_colorCombo->setMinimumWidth(112);
    m_colorCombo->addItem(QStringLiteral("随机颜色"));
    m_colorCombo->addItem(QStringLiteral("白色"), QColor(QStringLiteral("#FFFFFF")));
    m_colorCombo->addItem(QStringLiteral("红色"), QColor(QStringLiteral("#FF8B8B")));
    m_colorCombo->addItem(QStringLiteral("黄色"), QColor(QStringLiteral("#FFE08A")));
    m_colorCombo->addItem(QStringLiteral("绿色"), QColor(QStringLiteral("#94E6BA")));
    m_colorCombo->addItem(QStringLiteral("蓝色"), QColor(QStringLiteral("#9BD1FF")));
    inputLayout->addWidget(m_colorCombo);
    auto *sendButton = new QPushButton(QStringLiteral("发送"), inputCard);
    sendButton->setObjectName(QStringLiteral("SendButton"));
    inputLayout->addWidget(sendButton);
    layout->addWidget(inputCard);

    m_sampleTimer = new QTimer(this);
    m_sampleTimer->setInterval(1200);
    connect(m_sampleTimer, &QTimer::timeout, this, &DanmakuDemo::sendSample);
    connect(m_danmaku, &DanmakuWidget::settingsChanged, this, &DanmakuDemo::syncSettings);
    connect(m_danmaku, &DanmakuWidget::statisticsChanged, this, &DanmakuDemo::syncStatistics);
    connect(m_fontSlider, &QSlider::valueChanged, m_danmaku, &DanmakuWidget::setFontSize);
    connect(m_fontSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            m_danmaku, &DanmakuWidget::setFontSize);
    connect(m_laneSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            m_danmaku, &DanmakuWidget::setLaneCount);
    connect(m_opacitySlider, &QSlider::valueChanged, this, [this](int value) {
        m_danmaku->setOpacity(value / qreal(100));
    });
    connect(m_speedSlider, &QSlider::valueChanged, this, [this](int value) {
        m_danmaku->setScrollSpeed(value);
    });
    connect(m_speedSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [this](int value) {
        m_danmaku->setScrollSpeed(value);
    });
    connect(m_pauseButton, &QPushButton::toggled, m_danmaku, &DanmakuWidget::setPaused);
    connect(clearButton, &QPushButton::clicked, m_danmaku, &DanmakuWidget::clear);
    connect(m_autoSamples, &QCheckBox::toggled, this, &DanmakuDemo::updateSampleTimer);
    connect(m_input, &QLineEdit::returnPressed, this, &DanmakuDemo::sendInput);
    connect(sendButton, &QPushButton::clicked, this, &DanmakuDemo::sendInput);
    syncSettings();
}

DanmakuDemo::~DanmakuDemo()
{
    m_sampleTimer->stop();
    disconnect(m_sampleTimer, &QTimer::timeout, this, &DanmakuDemo::sendSample);
    m_displayed = false;
    disconnect(m_danmaku, &DanmakuWidget::settingsChanged, this, &DanmakuDemo::syncSettings);
    disconnect(m_danmaku, &DanmakuWidget::statisticsChanged, this, &DanmakuDemo::syncStatistics);
}

void DanmakuDemo::showEvent(QShowEvent *event)
{
    m_displayed = true;
    QWidget::showEvent(event);
    updateSampleTimer();
}

void DanmakuDemo::hideEvent(QHideEvent *event)
{
    m_displayed = false;
    updateSampleTimer();
    QWidget::hideEvent(event);
}

void DanmakuDemo::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange)
        updateSampleTimer();
}

void DanmakuDemo::syncSettings()
{
    const QSignalBlocker fontSliderBlocker(m_fontSlider);
    const QSignalBlocker fontSpinBlocker(m_fontSpin);
    const QSignalBlocker laneBlocker(m_laneSpin);
    const QSignalBlocker opacityBlocker(m_opacitySlider);
    const QSignalBlocker speedSliderBlocker(m_speedSlider);
    const QSignalBlocker speedSpinBlocker(m_speedSpin);
    const QSignalBlocker pauseBlocker(m_pauseButton);
    m_fontSlider->setValue(m_danmaku->fontSize());
    m_fontSpin->setValue(m_danmaku->fontSize());
    m_laneSpin->setValue(m_danmaku->laneCount());
    const int opacityPercent = qRound(m_danmaku->opacity() * 100);
    m_opacitySlider->setValue(opacityPercent);
    m_opacityValue->setText(QStringLiteral("%1%").arg(opacityPercent));
    const int speed = qRound(m_danmaku->scrollSpeed());
    m_speedSlider->setValue(speed);
    m_speedSpin->setValue(speed);
    m_pauseButton->setChecked(m_danmaku->isPaused());
    m_pauseButton->setText(m_danmaku->isPaused() ? QStringLiteral("继续") : QStringLiteral("暂停"));
    syncStatistics();
}

void DanmakuDemo::syncStatistics()
{
    const int effective = m_danmaku->effectiveLaneCount();
    const int requested = m_danmaku->laneCount();
    m_statistics->setText(QStringLiteral("在屏  %1 条\n排队  %2 条\n通道  %3 / %4（有效 / 请求）")
                          .arg(m_danmaku->visibleCount()).arg(m_danmaku->queuedCount())
                          .arg(effective).arg(requested));
    m_laneHint->setText(effective == 0
        ? QStringLiteral("画面高度不足，暂无可用通道；增大窗口或调小字号后继续。")
        : QStringLiteral("有效通道随画面高度和字号调整。"));
    updateSampleTimer();
}

bool DanmakuDemo::canGenerateSamples() const
{
    return m_displayed && !isMinimized() && m_autoSamples->isChecked()
        && !m_danmaku->isPaused() && m_danmaku->effectiveLaneCount() > 0;
}

void DanmakuDemo::updateSampleTimer()
{
    if (!canGenerateSamples()) {
        m_sampleTimer->stop();
        return;
    }

    if (!m_seeded) {
        // 首次可展示时播种一次；sendText 的统计信号可能同步回到这里。
        m_seeded = true;
        const QStringList &texts = sampleTexts();
        for (int i = 0; i < 5; ++i)
            m_danmaku->sendText(texts.at(i), randomTextColor());
    }
    if (!m_sampleTimer->isActive())
        m_sampleTimer->start();
}

void DanmakuDemo::sendSample()
{
    if (!canGenerateSamples()) {
        updateSampleTimer();
        return;
    }
    if (m_danmaku->queuedCount() != 0)
        return;

    const QStringList &texts = sampleTexts();
    const int index = QRandomGenerator::global()->bounded(texts.size());
    m_danmaku->sendText(texts.at(index), randomTextColor());
}

QColor DanmakuDemo::inputColor() const
{
    return m_colorCombo->currentIndex() == 0
        ? randomTextColor() : m_colorCombo->currentData().value<QColor>();
}

void DanmakuDemo::sendInput()
{
    if (m_danmaku->sendText(m_input->text(), inputColor())) {
        m_input->clear();
        m_input->setFocus(Qt::OtherFocusReason);
    }
}
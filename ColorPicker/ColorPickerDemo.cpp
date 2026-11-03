#include "ColorPickerDemo.h"
#include "ColorPickerWidget.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ColorPickerDemo::ColorPickerDemo(QWidget *parent) : QWidget(parent)
{
    setObjectName(QStringLiteral("ColorPickerDemo"));
    setWindowTitle(QStringLiteral("ColorPicker · 颜色选择器"));
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(560, 560);
    resize(720, 620);
    setStyleSheet(QStringLiteral(
        "QWidget#ColorPickerDemo { background: #F1F5F9; }"
        "QLabel#DemoTitle { color: #0F172A; font-size: 23px; font-weight: 600; }"
        "QLabel#DemoDescription { color: #64748B; font-size: 12px; }"
        "QFrame#ColorApplicationCard { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 9px; }"
        "QLabel#ApplicationLabel { color: #475569; font-size: 12px; }"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(8);
    auto *title = new QLabel(QStringLiteral("颜色选择器"), this);
    title->setObjectName(QStringLiteral("DemoTitle"));
    layout->addWidget(title);
    auto *description = new QLabel(
        QStringLiteral("拖动色板与滑条选色，方向键微调，Shift 加速。Tab 切换控件，Enter 提交 HEX。"), this);
    description->setObjectName(QStringLiteral("DemoDescription"));
    description->setWordWrap(true);
    layout->addWidget(description);

    auto *picker = new ColorPickerWidget(this);
    layout->addWidget(picker, 1);

    auto *applicationCard = new QFrame(this);
    applicationCard->setObjectName(QStringLiteral("ColorApplicationCard"));
    auto *applicationLayout = new QHBoxLayout(applicationCard);
    applicationLayout->setContentsMargins(12, 7, 12, 7);
    auto *applicationLabel = new QLabel(QStringLiteral("实时应用 · 按钮背景与透明度"), applicationCard);
    applicationLabel->setObjectName(QStringLiteral("ApplicationLabel"));
    applicationLayout->addWidget(applicationLabel, 1);
    auto *sampleButton = new QPushButton(QStringLiteral("选色效果"), applicationCard);
    sampleButton->setAccessibleName(QStringLiteral("当前颜色的按钮应用示例"));
    applicationLayout->addWidget(sampleButton);
    layout->addWidget(applicationCard);

    const auto applyColor = [sampleButton](const QColor &color) {
        const qreal opacity = color.alphaF();
        const qreal luminance = (0.299 * color.red() + 0.587 * color.green()
                                 + 0.114 * color.blue()) * opacity + 255 * (1 - opacity);
        const QString foreground = luminance > 150 ? QStringLiteral("#0F172A") : QStringLiteral("#FFFFFF");
        sampleButton->setStyleSheet(QStringLiteral(
            "QPushButton { background-color: rgba(%1, %2, %3, %4); color: %5;"
            " border: 1px solid #CBD5E1; border-radius: 5px; padding: 5px 22px; font-size: 12px; }"
            "QPushButton:focus { border-color: #2563EB; }")
            .arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()).arg(foreground));
        sampleButton->setToolTip(QStringLiteral("RGBA(%1, %2, %3, %4)")
                                 .arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
    };
    connect(picker, &ColorPickerWidget::colorChanged, this, applyColor);
    applyColor(picker->selectedColor());
}

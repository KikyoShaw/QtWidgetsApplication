#include "ScreenCaptureOverlay.h"

#include <QApplication>
#include <QButtonGroup>
#include <QCloseEvent>
#include <QComboBox>
#include <QFontMetrics>
#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QKeySequence>
#include <QLabel>
#include <QLineF>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QPalette>
#include <QPlainTextEdit>
#include <QPointer>
#include <QPushButton>
#include <QResizeEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QShortcut>
#include <QSizePolicy>
#include <QStyle>
#include <QStyleOptionButton>
#include <QTextDocument>
#include <QTextOption>
#include <cmath>

ScreenCaptureOverlay::ScreenCaptureOverlay(const QImage &frozenImage, QWidget *parent)
    : QWidget(parent, Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
      image(frozenImage)
{
    image.setDevicePixelRatio(1.0);
    composite = image;
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);

    toolbarScrollArea = new QScrollArea(this);
    toolbar = toolbarScrollArea;
    toolbar->setAttribute(Qt::WA_StyledBackground);
    toolbar->setStyleSheet(QStringLiteral(
        "QWidget { background: #252b34; color: white; border-radius: 5px; }"
        "QPushButton { padding: 5px 10px; background: #394250; border: 1px solid #536071; }"
        "QPushButton:hover { background: #4a586b; }"
        "QPushButton:pressed, QPushButton:checked { background: #263442; border-color: #71c8ff; }"
        "QPushButton:focus { border-color: #71c8ff; }"
        "QPushButton:disabled { color: #8993a2; }"
        "QComboBox { padding: 3px 6px; background: #394250; border: 1px solid #536071; }"));
    toolbarScrollArea->setFrameShape(QFrame::NoFrame);
    toolbarScrollArea->setWidgetResizable(false);
    toolbarScrollArea->setFocusPolicy(Qt::NoFocus);
    toolbarScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    toolbarScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    toolbarContent = new QWidget;
    toolbarScrollArea->setWidget(toolbarContent);
    toolbarLayout = new QGridLayout(toolbarContent);
    toolbarLayout->setContentsMargins(8, 8, 8, 8);
    toolbarLayout->setSpacing(6);
    toolbarLayout->setSizeConstraint(QLayout::SetNoConstraint);

    const auto makeButton = [this](const QString &text) {
        auto *button = new QPushButton(text, toolbarContent);
        button->setAutoDefault(false);
        button->setDefault(false);
        button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        button->setMinimumSize(0, 28);
        return button;
    };
    toolGroup = new QButtonGroup(this);
    toolGroup->setExclusive(true);
    const std::array<QString, 4> toolTexts = {{QStringLiteral("选择"), QStringLiteral("马赛克"),
                                             QStringLiteral("文本"), QStringLiteral("箭头")}};
    for (int i = 0; i < static_cast<int>(toolButtons.size()); ++i) {
        toolButtons[i] = makeButton(toolTexts[i]);
        toolButtons[i]->setCheckable(true);
        toolGroup->addButton(toolButtons[i], i);
        connect(toolButtons[i], &QPushButton::clicked, this,
                [this, i]() { setTool(static_cast<Tool>(i)); });
    }
    toolButtons[0]->setChecked(true);
    undoButton = makeButton(QStringLiteral("撤销"));
    clearButton = makeButton(QStringLiteral("清空"));
    connect(undoButton, &QPushButton::clicked, this, &ScreenCaptureOverlay::undoAnnotation);
    connect(clearButton, &QPushButton::clicked, this, &ScreenCaptureOverlay::clearAnnotations);

    colorLabel = new QLabel(QStringLiteral("颜色"), toolbarContent);
    colorCombo = new QComboBox(toolbarContent);
    colorCombo->addItem(QStringLiteral("红色"), QColor(Qt::red));
    colorCombo->addItem(QStringLiteral("黄色"), QColor(Qt::yellow));
    colorCombo->addItem(QStringLiteral("绿色"), QColor(Qt::green));
    colorCombo->addItem(QStringLiteral("蓝色"), QColor(Qt::blue));
    colorCombo->addItem(QStringLiteral("白色"), QColor(Qt::white));
    fontSizeLabel = new QLabel(QStringLiteral("字号"), toolbarContent);
    fontSizeCombo = new QComboBox(toolbarContent);
    for (int value : {12, 18, 24, 32, 48})
        fontSizeCombo->addItem(QString::number(value), value);
    fontSizeCombo->setCurrentIndex(2);
    lineWidthLabel = new QLabel(QStringLiteral("线宽"), toolbarContent);
    lineWidthCombo = new QComboBox(toolbarContent);
    for (int value : {2, 3, 5, 8})
        lineWidthCombo->addItem(QString::number(value), value);
    lineWidthCombo->setCurrentIndex(1);
    mosaicSizeLabel = new QLabel(QStringLiteral("粒度"), toolbarContent);
    mosaicSizeCombo = new QComboBox(toolbarContent);
    for (int value : {6, 12, 20, 32})
        mosaicSizeCombo->addItem(QString::number(value), value);
    mosaicSizeCombo->setCurrentIndex(1);
    for (QComboBox *combo : {colorCombo, fontSizeCombo, lineWidthCombo, mosaicSizeCombo}) {
        combo->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        combo->setMinimumHeight(28);
    }
    connect(colorCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int) { updateTextEditor(); });
    connect(fontSizeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            [this](int) { updateTextEditor(); });

    const std::array<QString, 4> texts = {{QStringLiteral("重选"), QStringLiteral("复制"),
                                         QStringLiteral("保存 PNG"), QStringLiteral("取消")}};
    for (int i = 0; i < static_cast<int>(buttons.size()); ++i)
        buttons[i] = makeButton(texts[i]);
    connect(buttons[0], &QPushButton::clicked, this, [this]() {
        resetSelection();
        emit reselectRequested();
    });
    connect(buttons[1], &QPushButton::clicked, this,
            [this]() { requestCapture(ScreenCaptureController::Copy); });
    connect(buttons[2], &QPushButton::clicked, this,
            [this]() { requestCapture(ScreenCaptureController::Save); });
    connect(buttons[3], &QPushButton::clicked, this, [this]() { requestCancel(true); });
    toolbar->hide();

    textEditor = new QPlainTextEdit(this);
    textEditor->setFrameShape(QFrame::NoFrame);
    textEditor->setContentsMargins(0, 0, 0, 0);
    textEditor->setStyleSheet(QStringLiteral(
        "QPlainTextEdit { background: rgba(25, 31, 40, 190); border: 0px; padding: 0px; }"));
    textEditor->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEditor->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEditor->setLineWrapMode(QPlainTextEdit::WidgetWidth);
    textEditor->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    textEditor->document()->setDocumentMargin(0);
    textEditor->installEventFilter(this);
    textEditor->viewport()->installEventFilter(this);
    connect(textEditor, &QPlainTextEdit::textChanged, this, &ScreenCaptureOverlay::fitTextEditor);
    // 输出按布局起点裁剪长文，编辑器也固定显示文档顶部。
    connect(textEditor->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int value) {
        if (textEditing && value != 0)
            textEditor->verticalScrollBar()->setValue(0);
    });
    textEditor->hide();

    copyShortcut = new QShortcut(QKeySequence(Qt::Key_Return), this);
    copyShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(copyShortcut, &QShortcut::activated, this,
            [this]() { requestCapture(ScreenCaptureController::Copy); });
    keypadCopyShortcut = new QShortcut(QKeySequence(Qt::Key_Enter), this);
    keypadCopyShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(keypadCopyShortcut, &QShortcut::activated, this,
            [this]() { requestCapture(ScreenCaptureController::Copy); });
    auto *saveShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
    saveShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(saveShortcut, &QShortcut::activated, this,
            [this]() { requestCapture(ScreenCaptureController::Save); });
    auto *cancelShortcut = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    cancelShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(cancelShortcut, &QShortcut::activated, this, [this]() { requestCancel(); });
    undoShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this);
    undoShortcut->setContext(Qt::WidgetWithChildrenShortcut);
    connect(undoShortcut, &QShortcut::activated, this, &ScreenCaptureOverlay::undoAnnotation);
    updateToolControls();
}

QRect ScreenCaptureOverlay::physicalRect() const
{
    if (image.isNull() || image.width() <= 0 || image.height() <= 0
        || width() <= 0 || height() <= 0 || selection.isEmpty()) {
        return QRect();
    }

    // 逻辑边界是半开区间；最终选区按实际截图尺寸映射到物理像素。
    const qreal ratioX = qreal(image.width()) / width();
    const qreal ratioY = qreal(image.height()) / height();
    const int left = qBound(0, static_cast<int>(std::floor(selection.left() * ratioX)), image.width() - 1);
    const int top = qBound(0, static_cast<int>(std::floor(selection.top() * ratioY)), image.height() - 1);
    const int right = qBound(left + 1, static_cast<int>(std::ceil(selection.right() * ratioX)), image.width());
    const int bottom = qBound(top + 1, static_cast<int>(std::ceil(selection.bottom() * ratioY)), image.height());
    return QRect(left, top, right - left, bottom - top);
}

QRect ScreenCaptureOverlay::toPhysicalRect(const QRectF &logicalRect) const
{
    if (image.isNull() || width() <= 0 || height() <= 0 || logicalRect.isEmpty())
        return QRect();
    const qreal ratioX = qreal(image.width()) / width();
    const qreal ratioY = qreal(image.height()) / height();
    const int left = qBound(0, static_cast<int>(std::floor(logicalRect.left() * ratioX)), image.width());
    const int top = qBound(0, static_cast<int>(std::floor(logicalRect.top() * ratioY)), image.height());
    const int right = qBound(left, static_cast<int>(std::ceil(logicalRect.right() * ratioX)), image.width());
    const int bottom = qBound(top, static_cast<int>(std::ceil(logicalRect.bottom() * ratioY)), image.height());
    return QRect(left, top, right - left, bottom - top);
}

QPointF ScreenCaptureOverlay::toPhysicalPoint(const QPointF &point) const
{
    if (width() <= 0 || height() <= 0)
        return QPointF();
    return QPointF(point.x() * image.width() / width(), point.y() * image.height() / height());
}

qreal ScreenCaptureOverlay::physicalSizeRatio() const
{
    if (width() <= 0 || height() <= 0)
        return 1.0;
    return (qreal(image.width()) / width() + qreal(image.height()) / height()) / 2.0;
}

QImage ScreenCaptureOverlay::resultImage() const
{
    const QRect pixels = physicalRect();
    if (pixels.isEmpty())
        return QImage();
    QImage result = composite.copy(pixels);
    result.setDevicePixelRatio(1.0);
    return result;
}

void ScreenCaptureOverlay::resetSelection()
{
    clearAnnotations();
    tool = Tool::Select;
    toolButtons[0]->setChecked(true);
    updateToolControls();
    drag = NoDrag;
    handle = NoHandle;
    selection = QRectF();
    initialSelection = QRectF();
    toolbar->hide();
    setCursor(Qt::CrossCursor);
    update();
}

QPointF ScreenCaptureOverlay::boundedPoint(const QPointF &point) const
{
    return QPointF(qBound(qreal(0), point.x(), qreal(qMax(0, width()))),
                   qBound(qreal(0), point.y(), qreal(qMax(0, height()))));
}

QPointF ScreenCaptureOverlay::boundedSelectionPoint(const QPointF &point) const
{
    if (selection.isEmpty())
        return boundedPoint(point);
    return QPointF(qBound(selection.left(), point.x(), selection.right()),
                   qBound(selection.top(), point.y(), selection.bottom()));
}

QSizeF ScreenCaptureOverlay::minimumSelectionSize() const
{
    if (image.width() <= 0 || image.height() <= 0 || width() <= 0 || height() <= 0)
        return QSizeF();
    return QSizeF(qreal(width()) / image.width(), qreal(height()) / image.height());
}

QRectF ScreenCaptureOverlay::newSelection(const QPointF &anchor, const QPointF &point) const
{
    const QSizeF minimum = minimumSelectionSize();
    if (minimum.isEmpty())
        return QRectF();

    qreal left = qMin(anchor.x(), point.x());
    qreal right = qMax(anchor.x(), point.x());
    qreal top = qMin(anchor.y(), point.y());
    qreal bottom = qMax(anchor.y(), point.y());
    if (right - left < minimum.width()) {
        if (point.x() < anchor.x()) {
            left = qMax(qreal(0), right - minimum.width());
            right = qMin(qreal(width()), left + minimum.width());
        } else {
            right = qMin(qreal(width()), left + minimum.width());
            left = qMax(qreal(0), right - minimum.width());
        }
    }
    if (bottom - top < minimum.height()) {
        if (point.y() < anchor.y()) {
            top = qMax(qreal(0), bottom - minimum.height());
            bottom = qMin(qreal(height()), top + minimum.height());
        } else {
            bottom = qMin(qreal(height()), top + minimum.height());
            top = qMax(qreal(0), bottom - minimum.height());
        }
    }
    return QRectF(left, top, right - left, bottom - top);
}

std::array<QPointF, 8> ScreenCaptureOverlay::handlePositions() const
{
    const qreal left = selection.left();
    const qreal right = selection.right();
    const qreal top = selection.top();
    const qreal bottom = selection.bottom();
    const qreal centerX = (left + right) / 2.0;
    const qreal centerY = (top + bottom) / 2.0;
    return {{QPointF(left, top), QPointF(centerX, top), QPointF(right, top),
             QPointF(right, centerY), QPointF(right, bottom), QPointF(centerX, bottom),
             QPointF(left, bottom), QPointF(left, centerY)}};
}

ScreenCaptureOverlay::Handle ScreenCaptureOverlay::hitHandle(const QPointF &point) const
{
    if (selection.isEmpty())
        return NoHandle;
    const auto positions = handlePositions();
    Handle nearest = NoHandle;
    qreal nearestDistance = 0.0;
    const Handle preferred = point.x() < selection.center().x()
        ? (point.y() < selection.center().y() ? TopLeft : BottomLeft)
        : (point.y() < selection.center().y() ? TopRight : BottomRight);
    for (int i = 0; i < static_cast<int>(positions.size()); ++i) {
        const QPointF delta = point - positions[i];
        if (std::abs(delta.x()) > 9.0 || std::abs(delta.y()) > 9.0)
            continue;
        const qreal distance = delta.x() * delta.x() + delta.y() * delta.y();
        if (nearest == NoHandle || distance < nearestDistance
            || (distance == nearestDistance && i == preferred)) {
            nearest = static_cast<Handle>(i);
            nearestDistance = distance;
        }
    }
    return nearest;
}

bool ScreenCaptureOverlay::containsSelection(const QPointF &point) const
{
    return !selection.isEmpty() && point.x() >= selection.left() && point.x() < selection.right()
        && point.y() >= selection.top() && point.y() < selection.bottom();
}

void ScreenCaptureOverlay::updateSelection(const QPointF &mousePoint)
{
    const QPointF point = boundedPoint(mousePoint);
    const QPointF delta = point - pressPoint;
    if (drag == CreateSelection) {
        selection = newSelection(pressPoint, point);
    } else if (drag == MoveSelection) {
        const qreal dx = qBound(-initialSelection.left(), delta.x(), qreal(width()) - initialSelection.right());
        const qreal dy = qBound(-initialSelection.top(), delta.y(), qreal(height()) - initialSelection.bottom());
        selection = initialSelection.translated(dx, dy);
    } else if (drag == ResizeSelection) {
        const QSizeF minimum = minimumSelectionSize();
        qreal left = initialSelection.left();
        qreal right = initialSelection.right();
        qreal top = initialSelection.top();
        qreal bottom = initialSelection.bottom();
        if (handle == TopLeft || handle == Left || handle == BottomLeft)
            left = qBound(qreal(0), initialSelection.left() + delta.x(), qMax(qreal(0), right - minimum.width()));
        if (handle == TopRight || handle == Right || handle == BottomRight)
            right = qBound(qMin(qreal(width()), left + minimum.width()), initialSelection.right() + delta.x(), qreal(width()));
        if (handle == TopLeft || handle == Top || handle == TopRight)
            top = qBound(qreal(0), initialSelection.top() + delta.y(), qMax(qreal(0), bottom - minimum.height()));
        if (handle == BottomLeft || handle == Bottom || handle == BottomRight)
            bottom = qBound(qMin(qreal(height()), top + minimum.height()), initialSelection.bottom() + delta.y(), qreal(height()));
        selection = QRectF(left, top, right - left, bottom - top);
    }
    update();
}

void ScreenCaptureOverlay::updateCursor(const QPointF &point)
{
    if (tool != Tool::Select) {
        setCursor(tool == Tool::Text && containsSelection(point) ? Qt::IBeamCursor : Qt::CrossCursor);
        return;
    }
    const Handle cursorHandle = drag == ResizeSelection ? handle : hitHandle(point);
    switch (cursorHandle) {
    case TopLeft:
    case BottomRight:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TopRight:
    case BottomLeft:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case Top:
    case Bottom:
        setCursor(Qt::SizeVerCursor);
        break;
    case Left:
    case Right:
        setCursor(Qt::SizeHorCursor);
        break;
    default:
        setCursor(drag == MoveSelection || containsSelection(point) ? Qt::SizeAllCursor : Qt::CrossCursor);
        break;
    }
}

void ScreenCaptureOverlay::arrangeToolbar()
{
    if (selection.isEmpty() || width() <= 0 || height() <= 0)
        return;

    const int margin = qMin(8, qMin(width() / 2, height() / 2));
    const int availableWidth = qMax(1, width() - margin * 2);
    const int availableHeight = qMax(1, height() - margin * 2);
    toolbar->ensurePolished();
    toolbarContent->ensurePolished();
    const QMargins contentsMargins = toolbarLayout->contentsMargins();
    const int horizontalMargins = contentsMargins.left() + contentsMargins.right();
    const int verticalMargins = contentsMargins.top() + contentsMargins.bottom();
    const int horizontalSpacing = qMax(0, toolbarLayout->horizontalSpacing());
    const int verticalSpacing = qMax(0, toolbarLayout->verticalSpacing());

    QVector<QPushButton *> allButtons;
    for (QPushButton *button : toolButtons)
        allButtons.append(button);
    allButtons.append(undoButton);
    allButtons.append(clearButton);
    for (QPushButton *button : buttons)
        allButtons.append(button);
    for (QPushButton *button : allButtons) {
        button->ensurePolished();
        QStyleOptionButton option;
        option.initFrom(button);
        option.text = button->text();
        const QSize textSize = button->fontMetrics().size(
            Qt::TextShowMnemonic | Qt::TextSingleLine, button->text());
        const QSize styledSize = button->style()->sizeFromContents(
            QStyle::CT_PushButton, &option, textSize, button);
        const QSize hint = button->sizeHint().expandedTo(button->minimumSizeHint())
                               .expandedTo(styledSize).expandedTo(QSize(0, 28));
        button->setMinimumSize(hint);
    }

    using Unit = QVector<QWidget *>;
    using Group = QVector<Unit>;
    QVector<Group> groups;
    Group tools;
    for (QPushButton *button : toolButtons)
        tools.append(Unit{button});
    tools.append(Unit{undoButton});
    tools.append(Unit{clearButton});
    groups.append(tools);
    Group parameters;
    if (!colorCombo->isHidden())
        parameters.append(Unit{colorLabel, colorCombo});
    if (!fontSizeCombo->isHidden())
        parameters.append(Unit{fontSizeLabel, fontSizeCombo});
    if (!lineWidthCombo->isHidden())
        parameters.append(Unit{lineWidthLabel, lineWidthCombo});
    if (!mosaicSizeCombo->isHidden())
        parameters.append(Unit{mosaicSizeLabel, mosaicSizeCombo});
    if (!parameters.isEmpty())
        groups.append(parameters);
    Group actions;
    for (QPushButton *button : buttons)
        actions.append(Unit{button});
    groups.append(actions);

    const auto layoutRows = [&](int widthLimit) {
        // 删除布局项保留控件，并清空旧行列约束，避免换行后残留空列宽度。
        while (QLayoutItem *item = toolbarLayout->takeAt(0))
            delete item;
        for (int column = 0; column < toolbarLayout->columnCount(); ++column) {
            toolbarLayout->setColumnMinimumWidth(column, 0);
            toolbarLayout->setColumnStretch(column, 0);
        }
        for (int row = 0; row < toolbarLayout->rowCount(); ++row) {
            toolbarLayout->setRowMinimumHeight(row, 0);
            toolbarLayout->setRowStretch(row, 0);
        }
        int rowCount = 0;
        int naturalWidth = 0;
        int naturalHeight = 0;
        const int innerWidth = qMax(1, widthLimit - horizontalMargins);
        for (const Group &group : groups) {
            QHBoxLayout *rowLayout = nullptr;
            int rowWidth = 0;
            int rowHeight = 0;
            const auto finishRow = [&]() {
                if (!rowLayout)
                    return;
                rowLayout->addStretch(1);
                naturalWidth = qMax(naturalWidth, rowWidth);
                if (rowCount > 1)
                    naturalHeight += verticalSpacing;
                naturalHeight += rowHeight;
                rowLayout = nullptr;
                rowWidth = 0;
                rowHeight = 0;
            };
            for (const Unit &unit : group) {
                int unitWidth = 0;
                int unitHeight = 0;
                for (QWidget *widget : unit) {
                    widget->ensurePolished();
                    const QSize hint = widget->sizeHint().expandedTo(widget->minimumSizeHint())
                                           .expandedTo(widget->minimumSize());
                    if (unitWidth > 0)
                        unitWidth += horizontalSpacing;
                    unitWidth += hint.width();
                    unitHeight = qMax(unitHeight, hint.height());
                }
                if (rowLayout && rowWidth + horizontalSpacing + unitWidth > innerWidth)
                    finishRow();
                if (!rowLayout) {
                    rowLayout = new QHBoxLayout;
                    rowLayout->setContentsMargins(0, 0, 0, 0);
                    rowLayout->setSpacing(horizontalSpacing);
                    toolbarLayout->addLayout(rowLayout, rowCount++, 0);
                } else {
                    rowWidth += horizontalSpacing;
                }
                for (QWidget *widget : unit)
                    rowLayout->addWidget(widget, 0, Qt::AlignVCenter);
                rowWidth += unitWidth;
                rowHeight = qMax(rowHeight, unitHeight);
            }
            finishRow();
        }
        return QSize(horizontalMargins + naturalWidth, verticalMargins + naturalHeight);
    };

    QSize contentSize = layoutRows(availableWidth);
    const int scrollbarExtent = toolbar->style()->pixelMetric(QStyle::PM_ScrollBarExtent, nullptr, toolbar);
    const bool verticalScroll = contentSize.height() > availableHeight;
    if (verticalScroll)
        contentSize = layoutRows(qMax(1, availableWidth - scrollbarExtent));
    const int toolbarWidth = qMin(availableWidth, contentSize.width() + (verticalScroll ? scrollbarExtent : 0));
    const bool horizontalScroll = contentSize.width() > toolbarWidth - (verticalScroll ? scrollbarExtent : 0);
    const int toolbarHeight = qMin(availableHeight,
        contentSize.height() + (horizontalScroll ? scrollbarExtent : 0));
    qreal x = selection.right() - toolbarWidth;
    qreal y = selection.bottom() + 12;
    if (y + toolbarHeight > height() - margin)
        y = selection.top() - toolbarHeight - 12;
    if (y < margin)
        y = selection.bottom() - toolbarHeight - 12;
    x = qBound(qreal(margin), x, qreal(width() - margin - toolbarWidth));
    y = qBound(qreal(margin), y, qreal(height() - margin - toolbarHeight));
    toolbarContent->resize(contentSize);
    toolbar->setGeometry(static_cast<int>(std::floor(x)), static_cast<int>(std::floor(y)),
                         toolbarWidth, toolbarHeight);
    toolbarLayout->activate();
}

void ScreenCaptureOverlay::showToolbar()
{
    if (selection.isEmpty() || drag != NoDrag || temporaryActive)
        return;
    arrangeToolbar();
    toolbar->show();
    toolbar->raise();
}

void ScreenCaptureOverlay::setTool(Tool nextTool)
{
    finishEditing(true, true);
    tool = nextTool;
    toolButtons[static_cast<int>(tool)]->setChecked(true);
    updateToolControls();
    showToolbar();
    updateCursor(selection.center());
    update();
}

void ScreenCaptureOverlay::updateToolControls()
{
    const bool colored = tool == Tool::Text || tool == Tool::Arrow;
    colorLabel->setVisible(colored);
    colorCombo->setVisible(colored);
    fontSizeLabel->setVisible(tool == Tool::Text);
    fontSizeCombo->setVisible(tool == Tool::Text);
    lineWidthLabel->setVisible(tool == Tool::Arrow);
    lineWidthCombo->setVisible(tool == Tool::Arrow);
    mosaicSizeLabel->setVisible(tool == Tool::Mosaic);
    mosaicSizeCombo->setVisible(tool == Tool::Mosaic);
    const bool hasWork = !annotations.isEmpty() || textEditing || temporaryActive;
    undoButton->setEnabled(hasWork);
    clearButton->setEnabled(hasWork);
}

QColor ScreenCaptureOverlay::annotationColor() const
{
    return colorCombo->currentData().value<QColor>();
}

void ScreenCaptureOverlay::rebuildComposite()
{
    composite = annotations.isEmpty() ? image : ScreenCaptureAnnotations::render(image, annotations);
    composite.setDevicePixelRatio(1.0);
    updateToolControls();
    update();
}

void ScreenCaptureOverlay::commitAnnotation(const ScreenCaptureAnnotations::Annotation &annotation)
{
    if (!ScreenCaptureAnnotations::isValid(annotation))
        return;
    annotations.append(annotation);
    rebuildComposite();
}

void ScreenCaptureOverlay::updateTemporaryAnnotation(const QPointF &mousePoint)
{
    if (!temporaryActive)
        return;
    const QPointF point = boundedSelectionPoint(mousePoint);
    temporaryAnnotation.end = toPhysicalPoint(point);
    if (tool == Tool::Mosaic) {
        temporaryAnnotation.rectangle = toPhysicalRect(QRectF(pressPoint, point).normalized())
                                            .intersected(temporaryAnnotation.clip);
    }
    temporaryPatch = ScreenCaptureAnnotations::preview(composite, temporaryAnnotation, &temporaryOrigin);
    update();
}

void ScreenCaptureOverlay::beginTextEditing(const QPointF &point)
{
    const QRect crop = selection.toAlignedRect().intersected(rect());
    const int left = static_cast<int>(std::ceil(selection.left()));
    const int top = static_cast<int>(std::ceil(selection.top()));
    const int right = static_cast<int>(std::floor(selection.right()));
    const int bottom = static_cast<int>(std::floor(selection.bottom()));
    const QRect inside = QRect(left, top, right - left, bottom - top).intersected(crop);
    if (inside.isEmpty())
        return;
    const int editorWidth = qMin(360, inside.width());
    const int editorHeight = qMin(90, inside.height());
    const int x = qBound(inside.left(), static_cast<int>(std::floor(point.x())),
                         inside.right() + 1 - editorWidth);
    const int y = qBound(inside.top(), static_cast<int>(std::floor(point.y())),
                         inside.bottom() + 1 - editorHeight);
    textClip = physicalRect();
    textEditor->clear();
    textEditor->setGeometry(x, y, editorWidth, editorHeight);
    textEditing = true;
    copyShortcut->setEnabled(false);
    keypadCopyShortcut->setEnabled(false);
    updateTextEditor();
    textEditor->show();
    textEditor->raise();
    showToolbar();
    textEditor->setFocus(Qt::MouseFocusReason);
    updateToolControls();
}

void ScreenCaptureOverlay::updateTextEditor()
{
    if (!textEditing || finishingEditing)
        return;
    QFont textFont = font();
    textFont.setPixelSize(fontSizeCombo->currentData().toInt());
    textEditor->setFont(textFont);
    textEditor->document()->setDefaultFont(textFont);
    QPalette palette = textEditor->palette();
    palette.setColor(QPalette::Text, annotationColor());
    textEditor->setPalette(palette);
    fitTextEditor();
}

void ScreenCaptureOverlay::fitTextEditor()
{
    if (!textEditing || finishingEditing || fittingTextEditor)
        return;
    fittingTextEditor = true;
    const int left = static_cast<int>(std::ceil(selection.left()));
    const int top = static_cast<int>(std::ceil(selection.top()));
    const int right = static_cast<int>(std::floor(selection.right()));
    const int bottom = static_cast<int>(std::floor(selection.bottom()));
    const QRect inside = QRect(left, top, right - left, bottom - top).intersected(rect());
    if (inside.isEmpty()) {
        fittingTextEditor = false;
        finishEditing(false, true);
        return;
    }
    {
        const int editorWidth = qMin(textEditor->width(), inside.width());
        const int x = qBound(inside.left(), textEditor->x(), inside.right() + 1 - editorWidth);
        const int y = qBound(inside.top(), textEditor->y(), inside.bottom());
        const int remainingHeight = inside.bottom() + 1 - y;
        QTextDocument document;
        document.setDocumentMargin(0);
        document.setDefaultFont(textEditor->font());
        QTextOption option = document.defaultTextOption();
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        document.setDefaultTextOption(option);
        document.setPlainText(textEditor->toPlainText());
        document.setTextWidth(qMax(1, editorWidth));
        const int minimumHeight = textEditor->fontMetrics().lineSpacing() * 2;
        const int documentHeight = static_cast<int>(std::ceil(document.size().height()));
        const int editorHeight = qMin(remainingHeight, qMax(minimumHeight, documentHeight));
        textEditor->setGeometry(x, y, editorWidth, qMax(1, editorHeight));
        textEditor->verticalScrollBar()->setValue(0);
    }
    fittingTextEditor = false;
}

void ScreenCaptureOverlay::finishEditing(bool commitText, bool cancelTemp)
{
    if (finishingEditing)
        return;
    finishingEditing = true;
    ScreenCaptureAnnotations::Annotation textAnnotation;
    bool addText = false;
    if (textEditing) {
        if (commitText && !textEditor->toPlainText().trimmed().isEmpty()) {
            textAnnotation.kind = ScreenCaptureAnnotations::Kind::Text;
            textAnnotation.clip = textClip;
            // 仅映射 viewport 内容，边框和编辑器装饰不属于文字布局。
            const QPoint origin = textEditor->viewport()->mapTo(this, QPoint(0, 0));
            const QRectF contentRect(QPointF(origin), QSizeF(textEditor->viewport()->size()));
            textAnnotation.rectangle = toPhysicalRect(contentRect).intersected(textClip);
            textAnnotation.text = textEditor->toPlainText();
            textAnnotation.color = annotationColor();
            textAnnotation.font = textEditor->font();
            const qreal ratioY = height() > 0 ? qreal(image.height()) / height() : 1.0;
            textAnnotation.font.setPixelSize(qMax(1, qRound(textEditor->font().pixelSize() * ratioY)));
            addText = true;
        }
        // 键盘事件仍在发送给此控件；隐藏并复用，不能同步销毁 sender。
        textEditing = false;
        textEditor->hide();
        textEditor->clear();
        textClip = QRect();
        copyShortcut->setEnabled(true);
        keypadCopyShortcut->setEnabled(true);
    }
    if (cancelTemp) {
        temporaryActive = false;
        temporaryAnnotation = ScreenCaptureAnnotations::Annotation();
        temporaryPatch = QImage();
    }
    if (addText)
        commitAnnotation(textAnnotation);
    finishingEditing = false;
    updateToolControls();
    update();
}

void ScreenCaptureOverlay::undoAnnotation()
{
    if (textEditing || temporaryActive) {
        finishEditing(false, true);
        showToolbar();
        return;
    }
    if (!annotations.isEmpty()) {
        annotations.removeLast();
        rebuildComposite();
    }
    showToolbar();
}

void ScreenCaptureOverlay::clearAnnotations()
{
    finishEditing(false, true);
    annotations.clear();
    composite = image;
    composite.setDevicePixelRatio(1.0);
    updateToolControls();
    update();
}

void ScreenCaptureOverlay::requestCancel(bool cancelSession)
{
    if (!cancelSession) {
        if (QWidget *popup = QApplication::activePopupWidget()) {
            popup->close();
            return;
        }
    }
    const bool unfinished = textEditing || temporaryActive;
    finishEditing(false, true);
    if (unfinished && !cancelSession) {
        showToolbar();
        setFocus(Qt::OtherFocusReason);
        return;
    }
    emit cancelRequested();
}

void ScreenCaptureOverlay::requestCapture(ScreenCaptureController::Action action)
{
    if (QApplication::activePopupWidget())
        return;
    finishEditing(true, true);
    showToolbar();
    if (isVisible() && drag == NoDrag && !selection.isEmpty())
        emit captureRequested(action);
}

bool ScreenCaptureOverlay::eventFilter(QObject *watched, QEvent *event)
{
    if (textEditing && (watched == textEditor || watched == textEditor->viewport())
        && (event->type() == QEvent::ShortcutOverride || event->type() == QEvent::KeyPress)) {
        auto *keyEvent = static_cast<QKeyEvent *>(event);
        const int key = keyEvent->key();
        const bool enter = key == Qt::Key_Return || key == Qt::Key_Enter;
        const bool control = keyEvent->modifiers().testFlag(Qt::ControlModifier);
        const bool save = control && key == Qt::Key_S;
        const bool undo = control && key == Qt::Key_Z;
        if (event->type() == QEvent::ShortcutOverride) {
            if (enter || key == Qt::Key_Escape || save || undo) {
                keyEvent->accept();
                return true;
            }
        } else if (key == Qt::Key_Escape) {
            finishEditing(false, true);
            showToolbar();
            setFocus(Qt::OtherFocusReason);
            keyEvent->accept();
            return true;
        } else if (enter && control) {
            finishEditing(true, true);
            showToolbar();
            setFocus(Qt::OtherFocusReason);
            keyEvent->accept();
            return true;
        } else if (save) {
            keyEvent->accept();
            requestCapture(ScreenCaptureController::Save);
            return true;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void ScreenCaptureOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    // 原图始终保留完整物理像素；缩放只用于屏幕显示，不参与最终裁剪。
    painter.drawImage(QRectF(rect()), image, QRectF(image.rect()));
    if (!selection.isEmpty()) {
        painter.save();
        painter.setClipRect(selection);
        painter.drawImage(QRectF(rect()), composite, QRectF(composite.rect()));
        if (temporaryActive && !temporaryPatch.isNull() && image.width() > 0 && image.height() > 0) {
            const qreal inverseX = qreal(width()) / image.width();
            const qreal inverseY = qreal(height()) / image.height();
            const QRectF target(temporaryOrigin.x() * inverseX, temporaryOrigin.y() * inverseY,
                                temporaryPatch.width() * inverseX, temporaryPatch.height() * inverseY);
            painter.drawImage(target, temporaryPatch, QRectF(temporaryPatch.rect()));
        }
        painter.restore();
    }
    QPainterPath shade;
    shade.setFillRule(Qt::OddEvenFill);
    shade.addRect(QRectF(rect()));
    if (!selection.isEmpty())
        shade.addRect(selection);
    painter.fillPath(shade, QColor(0, 0, 0, 145));

    if (selection.isEmpty()) {
        const QString hint = QStringLiteral("拖动选择区域");
        const QSize textSize = painter.fontMetrics().size(Qt::TextSingleLine, hint) + QSize(28, 18);
        const QSize boxSize(qMin(textSize.width(), width()), qMin(textSize.height(), height()));
        const QRect box(QPoint((width() - boxSize.width()) / 2, (height() - boxSize.height()) / 2), boxSize);
        painter.fillRect(box, QColor(25, 31, 40, 225));
        painter.setPen(Qt::white);
        painter.drawText(box, Qt::AlignCenter, hint);
        return;
    }

    painter.setPen(QPen(QColor(86, 190, 255), 1));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(selection);
    painter.setBrush(Qt::white);
    if (tool == Tool::Select) {
        for (const QPointF &point : handlePositions())
            painter.drawRect(QRectF(point.x() - 3, point.y() - 3, 6, 6));
    }

    const QRect pixels = physicalRect();
    const QString sizeText = QStringLiteral("%1 × %2 像素").arg(pixels.width()).arg(pixels.height());
    QSize labelSize = painter.fontMetrics().size(Qt::TextSingleLine, sizeText) + QSize(16, 10);
    labelSize.setWidth(qMin(labelSize.width(), width()));
    labelSize.setHeight(qMin(labelSize.height(), height()));
    qreal labelY = selection.top() - labelSize.height() - 8;
    if (labelY < 0)
        labelY = selection.top() + 10;
    const int x = static_cast<int>(qBound(qreal(0), selection.left(), qreal(width() - labelSize.width())));
    const int y = static_cast<int>(qBound(qreal(0), labelY, qreal(height() - labelSize.height())));
    const QRect label(QPoint(x, y), labelSize);
    painter.fillRect(label, QColor(25, 31, 40, 235));
    painter.setPen(Qt::white);
    painter.drawText(label, Qt::AlignCenter, sizeText);
}

void ScreenCaptureOverlay::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (!selection.isEmpty()) {
        selection = selection.intersected(QRectF(rect()));
        if (!selection.isEmpty())
            selection = newSelection(selection.topLeft(), selection.bottomRight());
    }
    if (selection.isEmpty()) {
        finishEditing(false, true);
        toolbar->hide();
    } else {
        if (temporaryActive)
            finishEditing(false, true);
        fitTextEditor();
        if (toolbar->isVisible())
            arrangeToolbar();
    }
    update();
}

void ScreenCaptureOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton || !isVisible() || image.isNull()) {
        event->ignore();
        return;
    }
    if (toolbar->isVisible() && toolbar->geometry().contains(event->localPos().toPoint())) {
        event->accept();
        return;
    }
    if (tool != Tool::Select && !containsSelection(event->localPos())) {
        event->ignore();
        return;
    }

    QPointer<ScreenCaptureOverlay> guard(this);
    emit selectionStarted();
    if (!guard || !isVisible())
        return;
    setFocus(Qt::MouseFocusReason);
    pressPoint = boundedPoint(event->localPos());
    if (tool != Tool::Select) {
        finishEditing(true, true);
        pressPoint = boundedSelectionPoint(pressPoint);
        if (tool == Tool::Text) {
            beginTextEditing(pressPoint);
        } else {
            temporaryAnnotation = ScreenCaptureAnnotations::Annotation();
            temporaryAnnotation.kind = tool == Tool::Mosaic
                ? ScreenCaptureAnnotations::Kind::Mosaic : ScreenCaptureAnnotations::Kind::Arrow;
            temporaryAnnotation.clip = physicalRect();
            temporaryAnnotation.start = toPhysicalPoint(pressPoint);
            temporaryAnnotation.end = temporaryAnnotation.start;
            temporaryAnnotation.color = annotationColor();
            temporaryAnnotation.lineWidth = lineWidthCombo->currentData().toInt() * physicalSizeRatio();
            temporaryAnnotation.mosaicSize = qMax(1,
                qRound(mosaicSizeCombo->currentData().toInt() * physicalSizeRatio()));
            temporaryActive = true;
            temporaryPatch = QImage();
            toolbar->hide();
            updateToolControls();
        }
        updateCursor(pressPoint);
        update();
        event->accept();
        return;
    }

    initialSelection = selection;
    handle = hitHandle(pressPoint);
    if (handle != NoHandle) {
        drag = ResizeSelection;
    } else if (containsSelection(pressPoint)) {
        drag = MoveSelection;
    } else {
        clearAnnotations();
        drag = CreateSelection;
        selection = newSelection(pressPoint, pressPoint);
    }
    toolbar->hide();
    updateCursor(pressPoint);
    update();
    event->accept();
}

void ScreenCaptureOverlay::mouseMoveEvent(QMouseEvent *event)
{
    if (temporaryActive)
        updateTemporaryAnnotation(event->localPos());
    else if (drag != NoDrag)
        updateSelection(event->localPos());
    updateCursor(boundedPoint(event->localPos()));
    event->accept();
}

void ScreenCaptureOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        event->ignore();
        return;
    }
    if (temporaryActive) {
        updateTemporaryAnnotation(event->localPos());
        const bool longEnough = tool != Tool::Arrow
            || QLineF(pressPoint, boundedSelectionPoint(event->localPos())).length() >= 4.0;
        const ScreenCaptureAnnotations::Annotation completed = temporaryAnnotation;
        finishEditing(false, true);
        if (longEnough)
            commitAnnotation(completed);
        showToolbar();
        updateCursor(boundedPoint(event->localPos()));
        event->accept();
        return;
    }
    if (drag == NoDrag) {
        event->ignore();
        return;
    }
    updateSelection(event->localPos());
    drag = NoDrag;
    handle = NoHandle;
    showToolbar();
    updateCursor(boundedPoint(event->localPos()));
    event->accept();
}

void ScreenCaptureOverlay::closeEvent(QCloseEvent *event)
{
    event->ignore();
    requestCancel(true);
}

#include "ScreenCaptureDemo.h"
#include "ScreenCaptureController.h"

#include <QApplication>
#include <QByteArray>
#include <QClipboard>
#include <QCloseEvent>
#include <QColor>
#include <QDateTime>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QImageWriter>
#include <QKeySequence>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPushButton>
#include <QSaveFile>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QVBoxLayout>

class ScreenCapturePreview final : public QWidget
{
public:
    explicit ScreenCapturePreview(const QImage &image, QWidget *parent)
        : QWidget(parent), m_image(image)
    {
        setMinimumHeight(180);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setAccessibleName(QStringLiteral("截图结果预览，棋盘格表示透明区域"));
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);

        const QRectF area = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);
        if (area.isEmpty())
            return;

        QPainterPath outline;
        outline.addRoundedRect(area, 7, 7);
        painter.save();
        painter.setClipPath(outline);
        painter.fillRect(rect(), QColor(QStringLiteral("#FFFFFF")));

        if (m_image.isNull()) {
            painter.setPen(QColor(QStringLiteral("#64748B")));
            painter.drawText(QRectF(rect()).adjusted(20, 20, -20, -20),
                             Qt::AlignCenter | Qt::TextWordWrap,
                             QStringLiteral("还没有截图\n点击“开始截图”，框选后即可在这里预览"));
        } else {
            const int tile = 12;
            const QColor tileColor(QStringLiteral("#E8EDF3"));
            for (int y = 0; y < height(); y += tile) {
                for (int x = 0; x < width(); x += tile) {
                    if ((x / tile + y / tile) % 2 == 0)
                        painter.fillRect(QRect(x, y, tile, tile), tileColor);
                }
            }

            const QRectF bounds = QRectF(rect()).adjusted(16, 16, -16, -16);
            if (!bounds.isEmpty()) {
                const QSizeF size = QSizeF(m_image.size()).scaled(bounds.size(), Qt::KeepAspectRatio);
                QRectF target(QPointF(0, 0), size);
                target.moveCenter(bounds.center());
                // 直接从原始像素绘制到当前布局，避免预缩放后再次缩放。
                painter.drawImage(target, m_image, QRectF(m_image.rect()));
            }
        }

        painter.restore();
        painter.setPen(QColor(QStringLiteral("#CBD5E1")));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(outline);
    }

private:
    const QImage &m_image;
};

namespace {

QString defaultSavePath()
{
    QString directory = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (directory.isEmpty())
        directory = QDir::homePath();

    const QString name = QStringLiteral("截图_%1.png")
                             .arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_HHmmss")));
    return QDir(directory).filePath(name);
}

} // namespace

ScreenCaptureDemo::ScreenCaptureDemo(QWidget *parent) : QWidget(parent)
{
    setObjectName(QStringLiteral("ScreenCaptureDemo"));
    setWindowTitle(QStringLiteral("ScreenCapture · 屏幕截图"));
    setAttribute(Qt::WA_StyledBackground, true);
    setMinimumSize(640, 500);
    resize(860, 660);
    setStyleSheet(QStringLiteral(
        "QWidget#ScreenCaptureDemo { background: #F1F5F9; }"
        "QLabel#DemoTitle { color: #0F172A; font-size: 23px; font-weight: 600; }"
        "QLabel#DemoDescription { color: #64748B; font-size: 12px; }"
        "QFrame#ResultCard { background: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 9px; }"
        "QLabel#ResultTitle { color: #334155; font-size: 14px; font-weight: 600; }"
        "QLabel#ResultSize { color: #64748B; font-size: 12px; }"
        "QPushButton { background: #FFFFFF; color: #334155; border: 1px solid #CBD5E1;"
        " border-radius: 6px; padding: 8px 18px; font-size: 13px; }"
        "QPushButton:hover { background: #F8FAFC; border-color: #94A3B8; }"
        "QPushButton:pressed { background: #E2E8F0; }"
        "QPushButton:focus { border-color: #2563EB; }"
        "QPushButton:disabled { background: #F8FAFC; color: #94A3B8; border-color: #E2E8F0; }"
        "QPushButton#StartCapture { background: #2563EB; color: #FFFFFF; border-color: #2563EB; }"
        "QPushButton#StartCapture:hover { background: #1D4ED8; border-color: #1D4ED8; }"
        "QPushButton#StartCapture:pressed { background: #1E40AF; border-color: #1E40AF; }"
        "QPushButton#StartCapture:disabled { background: #93B4F5; border-color: #93B4F5; }"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 18, 20, 18);
    layout->setSpacing(12);

    auto *title = new QLabel(QStringLiteral("屏幕截图"), this);
    title->setObjectName(QStringLiteral("DemoTitle"));
    layout->addWidget(title);

    auto *description = new QLabel(
        QStringLiteral("框选后可添加马赛克、文本和箭头；选区不跨屏。文字用 Ctrl+Enter 确认，Enter 换行。"), this);
    description->setObjectName(QStringLiteral("DemoDescription"));
    description->setWordWrap(true);
    layout->addWidget(description);

    auto *actions = new QHBoxLayout;
    actions->setSpacing(10);
    m_startButton = new QPushButton(QStringLiteral("开始截图"), this);
    m_startButton->setObjectName(QStringLiteral("StartCapture"));
    m_copyButton = new QPushButton(QStringLiteral("复制结果"), this);
    m_saveButton = new QPushButton(QStringLiteral("保存 PNG"), this);
    m_saveButton->setShortcut(QKeySequence::Save);
    m_saveButton->setToolTip(QStringLiteral("保存原始截图为 PNG（Ctrl+S）"));
    actions->addWidget(m_startButton);
    actions->addWidget(m_copyButton);
    actions->addWidget(m_saveButton);
    actions->addStretch(1);
    layout->addLayout(actions);

    auto *resultCard = new QFrame(this);
    resultCard->setObjectName(QStringLiteral("ResultCard"));
    auto *resultLayout = new QVBoxLayout(resultCard);
    resultLayout->setContentsMargins(14, 12, 14, 14);
    resultLayout->setSpacing(10);
    auto *resultHeader = new QHBoxLayout;
    auto *resultTitle = new QLabel(QStringLiteral("截图结果"), resultCard);
    resultTitle->setObjectName(QStringLiteral("ResultTitle"));
    m_sizeLabel = new QLabel(resultCard);
    m_sizeLabel->setObjectName(QStringLiteral("ResultSize"));
    m_sizeLabel->setTextFormat(Qt::PlainText);
    resultHeader->addWidget(resultTitle);
    resultHeader->addStretch(1);
    resultHeader->addWidget(m_sizeLabel);
    resultLayout->addLayout(resultHeader);
    m_preview = new ScreenCapturePreview(m_result, resultCard);
    resultLayout->addWidget(m_preview, 1);
    layout->addWidget(resultCard, 1);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setTextFormat(Qt::PlainText);
    m_statusLabel->setWordWrap(true);
    layout->addWidget(m_statusLabel);

    m_controller = new ScreenCaptureController(this);
    connect(m_startButton, &QPushButton::clicked, this, &ScreenCaptureDemo::startCapture);
    connect(m_copyButton, &QPushButton::clicked, this, &ScreenCaptureDemo::copyResult);
    connect(m_saveButton, &QPushButton::clicked, this, &ScreenCaptureDemo::savePNG);
    connect(m_controller, &ScreenCaptureController::captured, this,
            [this](const QImage &image, ScreenCaptureController::Action action) {
        updateCaptureUi(false);
        if (m_closing)
            return;
        if (image.isNull()) {
            setStatus(QStringLiteral("没有收到有效截图，请重新框选。"), true);
            return;
        }

        m_result = image;
        if (m_result.devicePixelRatio() != 1.0)
            m_result.setDevicePixelRatio(1.0);
        updateResultUi();
        setStatus(QStringLiteral("截图完成，可复制结果或保存 PNG。"));
        if (action == ScreenCaptureController::Copy)
            copyResult();
        else if (action == ScreenCaptureController::Save)
            savePNG();
    });
    connect(m_controller, &ScreenCaptureController::cancelled, this, [this] {
        updateCaptureUi(false);
        if (!m_closing)
            setStatus(QStringLiteral("已取消截图，已有结果仍可复制或保存。"));
    });
    connect(m_controller, &ScreenCaptureController::error, this, [this](const QString &message) {
        updateCaptureUi(false);
        if (!m_closing)
            setStatus(message, true);
    });

    updateResultUi();
    setStatus(QStringLiteral("准备就绪。点击“开始截图”选择屏幕区域。"));
}

void ScreenCaptureDemo::closeEvent(QCloseEvent *event)
{
    if (m_closing) {
        event->accept();
        return;
    }

    m_closing = true;
    m_controller->cancel();
    QWidget::closeEvent(event);
}

void ScreenCaptureDemo::startCapture()
{
    if (m_closing || m_controller->isActive())
        return;

    updateCaptureUi(true);
    setStatus(QStringLiteral("正在截图：框选后可加马赛克、文本和箭头；文字 Ctrl+Enter 确认，Esc 优先取消当前标注。"));
    if (!m_controller->start(this))
        updateCaptureUi(false);
}

void ScreenCaptureDemo::copyResult()
{
    if (m_closing || m_result.isNull() || m_controller->isActive())
        return;

    QClipboard *clipboard = QApplication::clipboard();
    if (!clipboard) {
        setStatus(QStringLiteral("剪贴板不可用，截图仍可保存为 PNG。"), true);
        return;
    }

    clipboard->setImage(m_result, QClipboard::Clipboard);
    setStatus(QStringLiteral("已复制截图（%1 × %2 像素）。")
                  .arg(m_result.width()).arg(m_result.height()));
}

void ScreenCaptureDemo::savePNG()
{
    if (m_closing || m_result.isNull() || m_controller->isActive())
        return;

    QString path = QFileDialog::getSaveFileName(this, QStringLiteral("保存截图"), defaultSavePath(),
                                               QStringLiteral("PNG 图片 (*.png)"), nullptr,
                                               QFileDialog::DontConfirmOverwrite);
    if (path.isEmpty()) {
        setStatus(QStringLiteral("已取消保存，截图仍可复制或保存。"));
        return;
    }

    const QString suffix = QFileInfo(path).suffix();
    if (suffix.isEmpty()) {
        path += QStringLiteral(".png");
    } else if (suffix.compare(QStringLiteral("png"), Qt::CaseInsensitive) != 0) {
        showSaveError(QStringLiteral("请使用 .png 扩展名保存截图。当前截图已保留，可重新保存。"));
        return;
    }

    const QFileInfo target(path);
    if (target.isDir()) {
        showSaveError(QStringLiteral("目标是文件夹，请选择 PNG 文件名。"));
        return;
    }
    if (target.exists()) {
        QMessageBox confirmation(QMessageBox::Question, QStringLiteral("确认覆盖"),
                                 QStringLiteral("文件已存在：\n%1\n\n是否覆盖？")
                                     .arg(QDir::toNativeSeparators(path)),
                                 QMessageBox::NoButton, this);
        confirmation.setTextFormat(Qt::PlainText);
        QPushButton *overwrite = confirmation.addButton(QStringLiteral("覆盖"), QMessageBox::AcceptRole);
        QPushButton *cancel = confirmation.addButton(QStringLiteral("取消"), QMessageBox::RejectRole);
        confirmation.setDefaultButton(cancel);
        confirmation.setEscapeButton(cancel);
        confirmation.exec();
        if (confirmation.clickedButton() != overwrite) {
            setStatus(QStringLiteral("未覆盖文件，截图仍可复制或保存。"));
            return;
        }
    }

    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        showSaveError(QStringLiteral("无法打开保存目标：\n%1\n%2")
                          .arg(QDir::toNativeSeparators(path), file.errorString()));
        return;
    }

    QImageWriter writer(&file, QByteArrayLiteral("png"));
    if (!writer.write(m_result)) {
        const QString reason = writer.errorString();
        file.cancelWriting();
        showSaveError(QStringLiteral("PNG 写入失败：\n%1\n%2")
                          .arg(QDir::toNativeSeparators(path), reason));
        return;
    }
    if (!file.commit()) {
        showSaveError(QStringLiteral("无法完成保存：\n%1\n%2")
                          .arg(QDir::toNativeSeparators(path), file.errorString()));
        return;
    }

    setStatus(QStringLiteral("已保存：%1（%2 × %3 像素）")
                  .arg(QDir::toNativeSeparators(path))
                  .arg(m_result.width()).arg(m_result.height()));
}

void ScreenCaptureDemo::updateCaptureUi(bool active)
{
    m_startButton->setEnabled(!active && !m_closing);
    const bool hasResult = !active && !m_closing && !m_result.isNull();
    m_copyButton->setEnabled(hasResult);
    m_saveButton->setEnabled(hasResult);
}

void ScreenCaptureDemo::updateResultUi()
{
    m_sizeLabel->setText(m_result.isNull()
                            ? QStringLiteral("尚无截图")
                            : QStringLiteral("%1 × %2 像素").arg(m_result.width()).arg(m_result.height()));
    m_preview->update();
    updateCaptureUi(m_controller->isActive());
}

void ScreenCaptureDemo::setStatus(const QString &text, bool error)
{
    m_statusLabel->setText(text);
    m_statusLabel->setStyleSheet(error ? QStringLiteral("color: #DC2626; font-size: 12px;")
                                     : QStringLiteral("color: #64748B; font-size: 12px;"));
}

void ScreenCaptureDemo::showSaveError(const QString &message)
{
    setStatus(message, true);
    QMessageBox messageBox(QMessageBox::Warning, QStringLiteral("保存 PNG"), message,
                           QMessageBox::NoButton, this);
    messageBox.setTextFormat(Qt::PlainText);
    messageBox.addButton(QStringLiteral("确定"), QMessageBox::AcceptRole);
    messageBox.exec();
}
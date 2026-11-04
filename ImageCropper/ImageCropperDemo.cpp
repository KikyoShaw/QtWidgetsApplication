#include "ImageCropperDemo.h"
#include "ImageCropperWidget.h"

#include <QByteArray>
#include <QColor>
#include <QComboBox>
#include <QDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QImage>
#include <QImageReader>
#include <QImageWriter>
#include <QKeySequence>
#include <QLabel>
#include <QLinearGradient>
#include <QMessageBox>
#include <QMimeData>
#include <QPainter>
#include <QPainterPath>
#include <QPaintEvent>
#include <QPointer>
#include <QPolygonF>
#include <QPushButton>
#include <QSaveFile>
#include <QShortcut>
#include <QSignalBlocker>
#include <QSizePolicy>
#include <QSplitter>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QStringList>
#include <QUrl>
#include <QVBoxLayout>
#include <QtMath>
#include <initializer_list>

class ImageCropperPreview final : public QWidget
{
public:
    explicit ImageCropperPreview(ImageCropperWidget *cropper, QWidget *parent)
        : QWidget(parent), m_cropper(cropper)
    {
        setMinimumWidth(200);
        setFixedHeight(240);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setAccessibleName(QStringLiteral("裁剪结果预览，棋盘格表示透明区域"));
    }

    void refresh()
    {
        m_image = QImage();
        update();
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const QRectF area = QRectF(rect()).adjusted(1, 1, -1, -1);
        QPainterPath outline;
        outline.addRoundedRect(area, 7, 7);
        painter.save();
        painter.setClipPath(outline);
        painter.fillRect(rect(), QColor(QStringLiteral("#FFFFFF")));
        const int tile = 12;
        for (int y = 0; y < height(); y += tile) {
            for (int x = 0; x < width(); x += tile) {
                if ((x / tile + y / tile) % 2 == 0)
                    painter.fillRect(QRect(x, y, tile, tile), QColor(QStringLiteral("#E2E8F0")));
            }
        }
        const QRectF bounds = QRectF(rect()).adjusted(8, 8, -8, -8);
        const QSize cropSize = m_cropper ? m_cropper->cropRect().size() : QSize();
        const qreal dpr = devicePixelRatioF();
        if (!cropSize.isEmpty() && !bounds.isEmpty()) {
            const QSizeF size = QSizeF(cropSize).scaled(bounds.size(), Qt::KeepAspectRatio);
            // 布局使用逻辑尺寸，预览按当前屏幕的物理像素生成。
            const QSize pixelSize(qMax(1, qCeil(size.width() * dpr)),
                                  qMax(1, qCeil(size.height() * dpr)));
            if (m_image.isNull() || m_pixelSize != pixelSize || m_image.devicePixelRatio() != dpr) {
                m_image = m_cropper->preview(pixelSize);
                m_image.setDevicePixelRatio(dpr);
                m_pixelSize = pixelSize;
            }
        } else {
            m_image = QImage();
        }
        if (!m_image.isNull()) {
            QRectF target(QPointF(0, 0), QSizeF(m_image.size()) / dpr);
            target.moveCenter(bounds.center());
            const QPointF position(qRound(target.x() * dpr) / dpr,
                                   qRound(target.y() * dpr) / dpr);
            painter.drawImage(position, m_image);
        } else {
            painter.setPen(QColor(QStringLiteral("#64748B")));
            painter.drawText(rect(), Qt::AlignCenter, QStringLiteral("暂无预览"));
        }
        painter.restore();
        painter.setPen(QColor(QStringLiteral("#CBD5E1")));
        painter.setBrush(Qt::NoBrush);
        painter.drawPath(outline);
    }

private:
    QPointer<ImageCropperWidget> m_cropper;
    QImage m_image;
    QSize m_pixelSize;
};

namespace {

QString picturesDirectory()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    return path.isEmpty() ? QDir::homePath() : path;
}

QString imageFileFilter()
{
    QStringList patterns;
    const auto formats = QImageReader::supportedImageFormats();
    for (const QByteArray &format : formats)
        patterns.append(QStringLiteral("*.%1").arg(QString::fromLatin1(format).toLower()));
    patterns.removeDuplicates();
    patterns.sort();
    if (patterns.isEmpty())
        return QStringLiteral("所有文件 (*)");
    return QStringLiteral("图片文件 (%1);;所有文件 (*)").arg(patterns.join(QLatin1Char(' ')));
}

QString firstLocalFile(const QMimeData *mimeData)
{
    if (!mimeData->hasUrls())
        return QString();
    const auto urls = mimeData->urls();
    for (const QUrl &url : urls) {
        if (url.isLocalFile())
            return url.toLocalFile();
    }
    return QString();
}

QString resolvedFilePath(const QString &path)
{
    const QFileInfo info(path);
    const QString canonical = info.canonicalFilePath();
    return QDir::cleanPath(canonical.isEmpty() ? info.absoluteFilePath() : canonical);
}

QImage sampleImage()
{
    QImage image(1600, 1000, QImage::Format_ARGB32_Premultiplied);
    if (image.isNull())
        return image;

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    const auto drawPolygon = [&painter](std::initializer_list<QPointF> points) {
        QPolygonF polygon;
        for (const QPointF &point : points)
            polygon.append(point);
        painter.drawPolygon(polygon);
    };
    QLinearGradient sky(0, 0, 0, 1000);
    sky.setColorAt(0, QColor(QStringLiteral("#CDE9F6")));
    sky.setColorAt(0.62, QColor(QStringLiteral("#FFE8C5")));
    sky.setColorAt(1, QColor(QStringLiteral("#F7CCAF")));
    painter.fillRect(image.rect(), sky);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(QStringLiteral("#FFD083")));
    painter.drawEllipse(QPointF(1210, 230), 108, 108);

    painter.setBrush(QColor(255, 255, 255, 175));
    QPainterPath cloud;
    cloud.addEllipse(QRectF(180, 150, 170, 62));
    cloud.addEllipse(QRectF(265, 120, 155, 95));
    cloud.addEllipse(QRectF(370, 155, 160, 57));
    painter.drawPath(cloud.simplified());
    painter.drawRoundedRect(QRectF(1060, 365, 360, 26), 13, 13);

    painter.setBrush(QColor(QStringLiteral("#9CB7CC")));
    drawPolygon({
        QPointF(0, 620), QPointF(280, 310), QPointF(455, 505),
        QPointF(750, 200), QPointF(1090, 565), QPointF(1310, 340),
        QPointF(1600, 620), QPointF(1600, 1000), QPointF(0, 1000)});
    painter.setBrush(QColor(QStringLiteral("#F7FAFC")));
    drawPolygon({
        QPointF(635, 320), QPointF(750, 200), QPointF(880, 340),
        QPointF(790, 302), QPointF(745, 332), QPointF(710, 285)});
    drawPolygon({
        QPointF(205, 395), QPointF(280, 310), QPointF(360, 400),
        QPointF(288, 370), QPointF(265, 390)});

    painter.setBrush(QColor(QStringLiteral("#5A8D9A")));
    drawPolygon({
        QPointF(0, 715), QPointF(210, 565), QPointF(430, 700),
        QPointF(680, 455), QPointF(1000, 710), QPointF(1290, 495),
        QPointF(1600, 685), QPointF(1600, 1000), QPointF(0, 1000)});
    QLinearGradient water(0, 620, 0, 1000);
    water.setColorAt(0, QColor(QStringLiteral("#87CBCD")));
    water.setColorAt(1, QColor(QStringLiteral("#C6E5D9")));
    QPainterPath lake;
    lake.moveTo(0, 755);
    lake.cubicTo(390, 670, 780, 700, 1600, 770);
    lake.lineTo(1600, 1000);
    lake.lineTo(0, 1000);
    lake.closeSubpath();
    painter.fillPath(lake, water);

    QPainterPath shore;
    shore.moveTo(0, 710);
    shore.cubicTo(200, 660, 420, 800, 610, 840);
    shore.cubicTo(850, 895, 1070, 890, 1170, 1000);
    shore.lineTo(0, 1000);
    shore.closeSubpath();
    painter.fillPath(shore, QColor(QStringLiteral("#3E746D")));
    painter.setBrush(QColor(QStringLiteral("#315C5C")));
    for (int i = 0; i < 5; ++i) {
        const qreal x = 1170 + i * 85;
        const qreal y = 810 + (i % 2) * 35;
        painter.drawRect(QRectF(x - 7, y, 14, 100));
        drawPolygon({
            QPointF(x, y - 140), QPointF(x - 62, y + 45), QPointF(x + 62, y + 45)});
    }

    QFont font = painter.font();
    font.setPixelSize(54);
    font.setWeight(QFont::DemiBold);
    painter.setFont(font);
    painter.setPen(QColor(QStringLiteral("#FFFFFF")));
    painter.drawText(QPointF(170, 835), QStringLiteral("山海之间"));
    font.setPixelSize(24);
    font.setWeight(QFont::Normal);
    painter.setFont(font);
    painter.drawText(QPointF(174, 882), QStringLiteral("留下喜欢的风景"));
    return image;
}

} // namespace

ImageCropperDemo::ImageCropperDemo(QWidget *parent) : QWidget(parent)
{
    setObjectName(QStringLiteral("ImageCropperDemo"));
    setWindowTitle(QStringLiteral("ImageCropper · 图片裁剪器"));
    setAttribute(Qt::WA_StyledBackground, true);
    setAcceptDrops(true);
    setMinimumSize(800, 600);
    resize(1080, 760);
    setStyleSheet(QStringLiteral(
        "QWidget#ImageCropperDemo { background: #F1F5F9; }"
        "QLabel#DemoTitle { color: #0F172A; font-size: 23px; font-weight: 600; }"
        "QLabel#DemoDescription { color: #64748B; font-size: 12px; }"
        "QFrame#CropToolbar, QFrame#CropSidebar { background: #FFFFFF;"
        " border: 1px solid #E2E8F0; border-radius: 9px; }"
        "QFrame#CropToolbar QPushButton, QFrame#CropToolbar QComboBox {"
        " color: #334155; background: #FFFFFF; border: 1px solid #CBD5E1;"
        " border-radius: 5px; padding: 6px 9px; font-size: 12px; }"
        "QFrame#CropToolbar QPushButton:hover { background: #F1F5F9; border-color: #94A3B8; }"
        "QFrame#CropToolbar QPushButton:focus, QFrame#CropToolbar QComboBox:focus { border-color: #2563EB; }"
        "QFrame#CropToolbar QPushButton:disabled { color: #94A3B8; background: #F8FAFC; }"
        "QFrame#CropToolbar QPushButton#ExportButton { color: #FFFFFF; background: #2563EB; border-color: #2563EB; }"
        "QFrame#CropToolbar QPushButton#ExportButton:hover { background: #1D4ED8; }"
        "QFrame#CropToolbar QPushButton#ExportButton:disabled { background: #94A3B8; border-color: #94A3B8; }"
        "QFrame#CropToolbar QLabel, QLabel#FieldTitle { color: #64748B; font-size: 12px; }"
        "QLabel#SideTitle { color: #0F172A; font-size: 15px; font-weight: 600; }"
        "QLabel#FieldValue { color: #334155; font-size: 13px; }"
        "QLabel#PreviewHint { color: #64748B; font-size: 12px; }"
        "QLabel#StatusLabel { font-size: 12px; }"
        "QSplitter::handle:horizontal { width: 10px; background: #F1F5F9; }"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(10);
    auto *title = new QLabel(QStringLiteral("图片裁剪器"), this);
    title->setObjectName(QStringLiteral("DemoTitle"));
    layout->addWidget(title);
    auto *description = new QLabel(
        QStringLiteral("调整选区与比例后导出；可拖入本地图片，仅载入第一张。"), this);
    description->setObjectName(QStringLiteral("DemoDescription"));
    description->setWordWrap(true);
    layout->addWidget(description);

    auto *toolbar = new QFrame(this);
    toolbar->setObjectName(QStringLiteral("CropToolbar"));
    auto *toolbarLayout = new QHBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(10, 8, 10, 8);
    toolbarLayout->setSpacing(7);
    auto *openButton = new QPushButton(QStringLiteral("打开图片"), toolbar);
    openButton->setToolTip(QStringLiteral("打开本地图片（Ctrl+O）"));
    toolbarLayout->addWidget(openButton);
    auto *sampleButton = new QPushButton(QStringLiteral("使用示例图"), toolbar);
    toolbarLayout->addWidget(sampleButton);
    m_fitButton = new QPushButton(QStringLiteral("适应窗口"), toolbar);
    m_fitButton->setToolTip(QStringLiteral("缩放并居中显示图片，保留当前选区。"));
    toolbarLayout->addWidget(m_fitButton);
    m_resetButton = new QPushButton(QStringLiteral("重置选区"), toolbar);
    m_resetButton->setToolTip(QStringLiteral("按当前比例居中重置选区，并适应窗口。"));
    toolbarLayout->addWidget(m_resetButton);
    toolbarLayout->addStretch();
    auto *aspectLabel = new QLabel(QStringLiteral("比例"), toolbar);
    toolbarLayout->addWidget(aspectLabel);
    m_aspectCombo = new QComboBox(toolbar);
    m_aspectCombo->setMinimumWidth(112);
    m_aspectCombo->setAccessibleName(QStringLiteral("裁剪比例"));
    m_aspectCombo->setToolTip(QStringLiteral("原图过小时，不支持的比例会禁用，当前图片保持不变。"));
    m_aspectCombo->addItem(QStringLiteral("自由比例"), int(ImageCropperWidget::Free));
    m_aspectCombo->addItem(QStringLiteral("1:1"), int(ImageCropperWidget::Square));
    m_aspectCombo->addItem(QStringLiteral("4:3"), int(ImageCropperWidget::Ratio4x3));
    m_aspectCombo->addItem(QStringLiteral("16:9"), int(ImageCropperWidget::Ratio16x9));
    m_aspectCombo->addItem(QStringLiteral("圆形头像"), int(ImageCropperWidget::Circle));
    aspectLabel->setBuddy(m_aspectCombo);
    toolbarLayout->addWidget(m_aspectCombo);
    m_exportButton = new QPushButton(QStringLiteral("导出图片"), toolbar);
    m_exportButton->setObjectName(QStringLiteral("ExportButton"));
    m_exportButton->setToolTip(QStringLiteral("另存裁剪结果（Ctrl+S）"));
    toolbarLayout->addWidget(m_exportButton);
    layout->addWidget(toolbar);

    auto *splitter = new QSplitter(Qt::Horizontal, this);
    splitter->setChildrenCollapsible(false);
    m_cropper = new ImageCropperWidget(splitter);
    splitter->addWidget(m_cropper);
    auto *sidebar = new QFrame(splitter);
    sidebar->setObjectName(QStringLiteral("CropSidebar"));
    sidebar->setMinimumWidth(256);
    sidebar->setMaximumWidth(300);
    auto *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(12, 12, 12, 12);
    sideLayout->setSpacing(8);
    auto *previewTitle = new QLabel(QStringLiteral("裁剪预览"), sidebar);
    previewTitle->setObjectName(QStringLiteral("SideTitle"));
    sideLayout->addWidget(previewTitle);
    m_preview = new ImageCropperPreview(m_cropper, sidebar);
    sideLayout->addWidget(m_preview);
    auto *previewHint = new QLabel(
        QStringLiteral("棋盘格表示透明区域。圆形头像导出为 PNG；JPEG 使用白色背景。"), sidebar);
    previewHint->setObjectName(QStringLiteral("PreviewHint"));
    previewHint->setWordWrap(true);
    sideLayout->addWidget(previewHint);

    const auto addField = [sidebar, sideLayout](const QString &text) {
        auto *label = new QLabel(text, sidebar);
        label->setObjectName(QStringLiteral("FieldTitle"));
        sideLayout->addWidget(label);
        auto *value = new QLabel(sidebar);
        value->setObjectName(QStringLiteral("FieldValue"));
        value->setTextFormat(Qt::PlainText);
        value->setWordWrap(true);
        value->setTextInteractionFlags(Qt::TextSelectableByMouse);
        sideLayout->addWidget(value);
        return value;
    };
    m_sourceLabel = addField(QStringLiteral("当前图片"));
    m_originalSizeLabel = addField(QStringLiteral("原图尺寸"));
    m_cropLabel = addField(QStringLiteral("选区（原图像素）"));
    m_zoomLabel = addField(QStringLiteral("视图缩放"));
    sideLayout->addStretch();
    splitter->addWidget(sidebar);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);
    splitter->setSizes(QList<int>{758, 266});
    layout->addWidget(splitter, 1);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("StatusLabel"));
    m_statusLabel->setTextFormat(Qt::PlainText);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(m_statusLabel);

    connect(openButton, &QPushButton::clicked, this, &ImageCropperDemo::openImage);
    connect(sampleButton, &QPushButton::clicked, this, &ImageCropperDemo::useSampleImage);
    connect(m_fitButton, &QPushButton::clicked, m_cropper, &ImageCropperWidget::fitToWindow);
    connect(m_resetButton, &QPushButton::clicked, m_cropper, &ImageCropperWidget::reset);
    connect(m_exportButton, &QPushButton::clicked, this, &ImageCropperDemo::exportImage);
    connect(m_aspectCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index < 0)
            return;
        const auto mode = static_cast<ImageCropperWidget::AspectMode>(m_aspectCombo->itemData(index).toInt());
        if (!m_cropper->setAspectMode(mode)) {
            updateAspectUi();
            setStatus(QStringLiteral("原图尺寸过小，无法使用该比例；当前图片和选区保持不变。"), true);
        }
    });
    // 状态提示独立于信号刷新，保留加载失败和导出结果。
    connect(m_cropper, &ImageCropperWidget::imageChanged, this, &ImageCropperDemo::updateImageUi);
    connect(m_cropper, &ImageCropperWidget::cropChanged, this, [this](const QRect &) {
        updateCropUi();
    });
    connect(m_cropper, &ImageCropperWidget::zoomChanged, this, [this](qreal) {
        updateZoomUi();
    });
    connect(m_cropper, &ImageCropperWidget::aspectModeChanged, this,
            [this](ImageCropperWidget::AspectMode) {
                updateAspectUi();
                updateCropUi();
            });

    auto *openShortcut = new QShortcut(QKeySequence(QKeySequence::Open), this);
    openShortcut->setContext(Qt::WindowShortcut);
    connect(openShortcut, &QShortcut::activated, this, &ImageCropperDemo::openImage);
    auto *saveShortcut = new QShortcut(QKeySequence(QKeySequence::Save), this);
    saveShortcut->setContext(Qt::WindowShortcut);
    connect(saveShortcut, &QShortcut::activated, this, &ImageCropperDemo::exportImage);

    useSampleImage();
}

void ImageCropperDemo::openImage()
{
    const QString directory = m_sourcePath.isEmpty()
        ? picturesDirectory() : QFileInfo(m_sourcePath).absolutePath();
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("打开图片"), directory, imageFileFilter());
    if (!path.isEmpty())
        loadImage(path);
}

void ImageCropperDemo::loadImage(const QString &path)
{
    QImageReader reader(path);
    reader.setAutoTransform(true);
    const QImage image = reader.read();
    if (image.isNull() || reader.error() != QImageReader::UnknownError) {
        setStatus(QStringLiteral("无法读取“%1”：%2。当前图片保持不变。")
                  .arg(QFileInfo(path).fileName(), reader.errorString()), true);
        return;
    }
    m_sourcePath = QFileInfo(path).absoluteFilePath();
    m_cropper->setImage(image);
    updateImageUi();
    setStatus(QStringLiteral("已载入“%1”（%2 × %3 像素）。")
              .arg(QFileInfo(path).fileName()).arg(image.width()).arg(image.height()));
}

void ImageCropperDemo::useSampleImage()
{
    const QImage image = sampleImage();
    if (image.isNull()) {
        setStatus(QStringLiteral("无法创建示例图，当前图片保持不变。"), true);
        return;
    }
    m_sourcePath.clear();
    m_cropper->setImage(image);
    updateImageUi();
    setStatus(QStringLiteral("已载入示例图，可调整选区后另存裁剪结果。"));
}

void ImageCropperDemo::updateImageUi()
{
    const QSize size = m_cropper->imageSize();
    const bool hasImage = !size.isEmpty();
    m_fitButton->setEnabled(hasImage);
    m_resetButton->setEnabled(hasImage);
    m_exportButton->setEnabled(hasImage);
    m_aspectCombo->setEnabled(hasImage);
    m_sourceLabel->setText(!hasImage ? QStringLiteral("未载入图片")
        : m_sourcePath.isEmpty() ? QStringLiteral("内置示例图") : QFileInfo(m_sourcePath).fileName());
    m_sourceLabel->setToolTip(m_sourcePath);
    m_originalSizeLabel->setText(hasImage
        ? QStringLiteral("%1 × %2 像素").arg(size.width()).arg(size.height())
        : QStringLiteral("—"));
    updateAspectUi();
    updateCropUi();
    updateZoomUi();
}

void ImageCropperDemo::updateCropUi()
{
    const QRect crop = m_cropper->cropRect();
    m_cropLabel->setText(crop.isEmpty() ? QStringLiteral("—")
        : QStringLiteral("位置：x %1，y %2\n尺寸：w %3，h %4")
            .arg(crop.x()).arg(crop.y()).arg(crop.width()).arg(crop.height()));
    m_preview->refresh();
}

void ImageCropperDemo::updateAspectUi()
{
    const QSignalBlocker blocker(m_aspectCombo);
    auto *model = qobject_cast<QStandardItemModel *>(m_aspectCombo->model());
    for (int i = 0; i < m_aspectCombo->count(); ++i) {
        const auto mode = static_cast<ImageCropperWidget::AspectMode>(m_aspectCombo->itemData(i).toInt());
        const bool available = m_cropper->isAspectModeAvailable(mode);
        if (model) {
            if (auto *item = model->item(i)) {
                item->setEnabled(available);
                item->setToolTip(available ? QString()
                    : QStringLiteral("原图尺寸不足，无法使用此比例；当前图片不会改变。"));
            }
        }
    }
    m_aspectCombo->setCurrentIndex(m_aspectCombo->findData(int(m_cropper->aspectMode())));
}

void ImageCropperDemo::updateZoomUi()
{
    m_zoomLabel->setText(m_cropper->imageSize().isEmpty() ? QStringLiteral("—")
        : QStringLiteral("%1%").arg(m_cropper->zoomFactor() * 100.0, 0, 'f', 1));
}

void ImageCropperDemo::setStatus(const QString &text, bool error)
{
    m_statusLabel->setText(text);
    m_statusLabel->setStyleSheet(error ? QStringLiteral("color: #B91C1C;")
                                     : QStringLiteral("color: #475569;"));
}

void ImageCropperDemo::dragEnterEvent(QDragEnterEvent *event)
{
    if (!firstLocalFile(event->mimeData()).isEmpty()) {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void ImageCropperDemo::dropEvent(QDropEvent *event)
{
    const QString path = firstLocalFile(event->mimeData());
    if (path.isEmpty()) {
        event->ignore();
        return;
    }
    event->setDropAction(Qt::CopyAction);
    event->accept();
    loadImage(path);
}

void ImageCropperDemo::exportImage()
{
    if (m_cropper->imageSize().isEmpty() || m_cropper->cropRect().isEmpty()) {
        setStatus(QStringLiteral("请先载入图片并选择裁剪区域。"), true);
        return;
    }

    const bool circle = m_cropper->aspectMode() == ImageCropperWidget::Circle;
    const QString pngFilter = QStringLiteral("PNG 图片 (*.png)");
    const QString jpegFilter = QStringLiteral("JPEG 图片 (*.jpg *.jpeg)");
    const QFileInfo source(m_sourcePath);
    const QString sourceSuffix = source.suffix().toLower();
    const bool defaultJpeg = !circle && (sourceSuffix == QStringLiteral("jpg")
                                        || sourceSuffix == QStringLiteral("jpeg"));
    const QString defaultSuffix = defaultJpeg ? sourceSuffix : QStringLiteral("png");
    QString selectedFilter = defaultJpeg ? jpegFilter : pngFilter;
    const QString baseName = m_sourcePath.isEmpty() ? QStringLiteral("示例图") : source.completeBaseName();
    const QString directory = m_sourcePath.isEmpty() ? picturesDirectory() : source.absolutePath();
    const QString initialPath = QDir(directory).filePath(
        QStringLiteral("%1_裁剪.%2").arg(baseName, defaultSuffix));
    const QString filters = circle ? pngFilter : pngFilter + QStringLiteral(";;") + jpegFilter;
    QString path = QFileDialog::getSaveFileName(this, QStringLiteral("导出裁剪图片"),
        initialPath, filters, &selectedFilter, QFileDialog::DontConfirmOverwrite);
    if (path.isEmpty())
        return;

    QString suffix = QFileInfo(path).suffix().toLower();
    if (suffix.isEmpty()) {
        suffix = !circle && selectedFilter == jpegFilter ? QStringLiteral("jpg") : QStringLiteral("png");
        path += QLatin1Char('.') + suffix;
    }
    const bool jpeg = suffix == QStringLiteral("jpg") || suffix == QStringLiteral("jpeg");
    if (circle && jpeg) {
        setStatus(QStringLiteral("圆形头像需要透明背景，请将扩展名改为 .png 后重新导出。"), true);
        return;
    }
    if (suffix != QStringLiteral("png") && !jpeg) {
        setStatus(QStringLiteral("不支持此扩展名，请使用 .png、.jpg 或 .jpeg。"), true);
        return;
    }

    path = QFileInfo(path).absoluteFilePath();
    if (!m_sourcePath.isEmpty()
        && resolvedFilePath(path).compare(resolvedFilePath(m_sourcePath), Qt::CaseInsensitive) == 0) {
        setStatus(QStringLiteral("不能覆盖当前原图，请使用不同的文件名另存。"), true);
        return;
    }
    const QFileInfo destination(path);
    if (destination.isDir()) {
        setStatus(QStringLiteral("目标路径是文件夹，请选择图片文件名。"), true);
        return;
    }
    // 补全扩展名后统一确认最终路径，避免绕过文件对话框的覆盖提示。
    if (destination.exists()) {
        QMessageBox confirmation(QMessageBox::Question, QStringLiteral("确认覆盖"),
            QStringLiteral("“%1”已存在，是否覆盖？").arg(destination.fileName()),
            QMessageBox::NoButton, this);
        confirmation.setTextFormat(Qt::PlainText);
        auto *overwriteButton = confirmation.addButton(QStringLiteral("覆盖"), QMessageBox::AcceptRole);
        auto *cancelButton = confirmation.addButton(QStringLiteral("取消"), QMessageBox::RejectRole);
        confirmation.setDefaultButton(cancelButton);
        confirmation.setEscapeButton(cancelButton);
        confirmation.exec();
        if (confirmation.clickedButton() != overwriteButton)
            return;
    }

    QImage image = m_cropper->croppedImage();
    if (image.isNull()) {
        setStatus(QStringLiteral("无法生成裁剪结果，图片未导出。"), true);
        return;
    }
    if (jpeg) {
        QImage opaque(image.size(), QImage::Format_RGB32);
        if (opaque.isNull()) {
            setStatus(QStringLiteral("无法创建 JPEG 图片，图片未导出。"), true);
            return;
        }
        opaque.fill(Qt::white);
        QPainter painter(&opaque);
        painter.drawImage(opaque.rect(), image, image.rect());
        painter.end();
        image = opaque;
    }

    QSaveFile output(path, this);
    output.setDirectWriteFallback(false);
    if (!output.open(QIODevice::WriteOnly)) {
        setStatus(QStringLiteral("无法创建导出文件：%1").arg(output.errorString()), true);
        return;
    }
    QImageWriter writer(&output, jpeg ? QByteArrayLiteral("jpeg") : QByteArrayLiteral("png"));
    writer.setQuality(95);
    if (!writer.write(image)) {
        const QString error = writer.errorString();
        output.cancelWriting();
        setStatus(QStringLiteral("导出失败：%1").arg(error), true);
        return;
    }
    if (!output.commit()) {
        const QString error = output.errorString();
        output.cancelWriting();
        setStatus(QStringLiteral("无法完成文件保存：%1").arg(error), true);
        return;
    }
    setStatus(QStringLiteral("已导出 %1 × %2 像素 · %3")
              .arg(image.width()).arg(image.height()).arg(QFileInfo(path).fileName()));
}

#include "NodeEditorDemo.h"
#include "NodeEditorWidget.h"

#include <QAbstractButton>
#include <QByteArray>
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QKeySequence>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSaveFile>
#include <QScopedValueRollback>
#include <QShortcut>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QVariant>
#include <QVBoxLayout>

namespace {

QJsonObject emptyGraph()
{
    QJsonObject object;
    object.insert(QStringLiteral("version"), 1);
    object.insert(QStringLiteral("nodes"), QJsonArray());
    object.insert(QStringLiteral("edges"), QJsonArray());
    return object;
}

QJsonObject exampleGraph()
{
    const auto makeNode = [](const QString &id, const QString &kind,
                             const QString &title, qreal x, qreal y) {
        QJsonObject node;
        node.insert(QStringLiteral("id"), id);
        node.insert(QStringLiteral("kind"), kind);
        node.insert(QStringLiteral("title"), title);
        node.insert(QStringLiteral("x"), x);
        node.insert(QStringLiteral("y"), y);
        return node;
    };
    const auto makeEdge = [](const QString &id, const QString &fromNode,
                             const QString &fromPort, const QString &toNode,
                             const QString &toPort) {
        QJsonObject from;
        from.insert(QStringLiteral("node"), fromNode);
        from.insert(QStringLiteral("port"), fromPort);
        QJsonObject to;
        to.insert(QStringLiteral("node"), toNode);
        to.insert(QStringLiteral("port"), toPort);
        QJsonObject edge;
        edge.insert(QStringLiteral("id"), id);
        edge.insert(QStringLiteral("from"), from);
        edge.insert(QStringLiteral("to"), to);
        return edge;
    };

    QJsonArray nodes;
    nodes.append(makeNode(QStringLiteral("input-a"), QStringLiteral("input"),
                          QStringLiteral("数据源 A"), -360.0, -150.0));
    nodes.append(makeNode(QStringLiteral("input-b"), QStringLiteral("input"),
                          QStringLiteral("数据源 B"), -360.0, 100.0));
    nodes.append(makeNode(QStringLiteral("process"), QStringLiteral("process"),
                          QStringLiteral("合并处理"), -20.0, -25.0));
    nodes.append(makeNode(QStringLiteral("output"), QStringLiteral("output"),
                          QStringLiteral("输出结果"), 320.0, -25.0));

    QJsonArray edges;
    edges.append(makeEdge(QStringLiteral("edge-a"), QStringLiteral("input-a"),
                          QStringLiteral("out"), QStringLiteral("process"),
                          QStringLiteral("a")));
    edges.append(makeEdge(QStringLiteral("edge-b"), QStringLiteral("input-b"),
                          QStringLiteral("out"), QStringLiteral("process"),
                          QStringLiteral("b")));
    edges.append(makeEdge(QStringLiteral("edge-out"), QStringLiteral("process"),
                          QStringLiteral("out"), QStringLiteral("output"),
                          QStringLiteral("in")));

    QJsonObject object = emptyGraph();
    object.insert(QStringLiteral("nodes"), nodes);
    object.insert(QStringLiteral("edges"), edges);
    return object;
}

QString documentsDirectory()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    return path.isEmpty() ? QDir::homePath() : path;
}

QString resolvedPath(const QString &path)
{
    const QFileInfo info(path);
    const QString canonical = info.canonicalFilePath();
    return QDir::cleanPath(canonical.isEmpty() ? info.absoluteFilePath() : canonical);
}

bool sameFile(const QString &first, const QString &second)
{
    if (first.isEmpty() || second.isEmpty())
        return false;
#ifdef Q_OS_WIN
    return resolvedPath(first).compare(resolvedPath(second), Qt::CaseInsensitive) == 0;
#else
    return resolvedPath(first) == resolvedPath(second);
#endif
}

} // namespace

NodeEditorDemo::NodeEditorDemo(QWidget *parent) : QWidget(parent)
{
    setObjectName(QStringLiteral("NodeEditorDemo"));
    setAttribute(Qt::WA_StyledBackground, true);
    setFont(QFont(QStringLiteral("Microsoft YaHei"), 9));
    setMinimumSize(900, 620);
    resize(1140, 780);
    setStyleSheet(QStringLiteral(
        "QWidget#NodeEditorDemo { background: #F1F5F9; }"
        "QLabel#DemoTitle { color: #0F172A; font-size: 23px; font-weight: 600; }"
        "QLabel#DemoDescription, QLabel#NodeInstructions { color: #64748B; font-size: 12px; }"
        "QFrame#NodeToolbar, QFrame#NodeStatusCard { background: #FFFFFF;"
        " border: 1px solid #E2E8F0; border-radius: 9px; }"
        "QFrame#NodeToolbar QPushButton { color: #334155; background: #FFFFFF;"
        " border: 1px solid #CBD5E1; border-radius: 5px; padding: 6px 12px; font-size: 12px; }"
        "QFrame#NodeToolbar QPushButton:hover { background: #F1F5F9; border-color: #94A3B8; }"
        "QFrame#NodeToolbar QPushButton:focus { border-color: #2563EB; }"
        "QFrame#NodeToolbar QPushButton[primary=\"true\"] { color: #FFFFFF;"
        " background: #2563EB; border-color: #2563EB; }"
        "QFrame#NodeToolbar QPushButton[primary=\"true\"]:hover { background: #1D4ED8; }"
        "QFrame#NodeToolbar QPushButton[primary=\"true\"]:focus { border-color: #1E40AF; }"
        "QFrame#NodeStatusCard QLabel { color: #475569; font-size: 12px; }"));

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 14, 16, 14);
    layout->setSpacing(10);
    auto *title = new QLabel(QStringLiteral("节点编辑器"), this);
    title->setObjectName(QStringLiteral("DemoTitle"));
    layout->addWidget(title);
    auto *description = new QLabel(
        QStringLiteral("创建节点与连线，编辑图结构，并通过 JSON 保存或载入。"), this);
    description->setObjectName(QStringLiteral("DemoDescription"));
    description->setWordWrap(true);
    layout->addWidget(description);

    auto *toolbar = new QFrame(this);
    toolbar->setObjectName(QStringLiteral("NodeToolbar"));
    auto *toolbarLayout = new QVBoxLayout(toolbar);
    toolbarLayout->setContentsMargins(10, 9, 10, 9);
    toolbarLayout->setSpacing(8);
    auto *nodeRow = new QHBoxLayout;
    nodeRow->setSpacing(7);
    toolbarLayout->addLayout(nodeRow);
    auto *fileRow = new QHBoxLayout;
    fileRow->setSpacing(7);
    toolbarLayout->addLayout(fileRow);
    const auto addButton = [toolbar](QHBoxLayout *row, const QString &text, bool primary) {
        auto *button = new QPushButton(text, toolbar);
        button->setProperty("primary", primary);
        row->addWidget(button);
        return button;
    };
    auto *inputButton = addButton(nodeRow, QStringLiteral("添加输入"), true);
    auto *processButton = addButton(nodeRow, QStringLiteral("添加处理"), true);
    auto *outputButton = addButton(nodeRow, QStringLiteral("添加输出"), true);
    auto *deleteButton = addButton(nodeRow, QStringLiteral("删除所选"), false);
    deleteButton->setToolTip(QStringLiteral("删除所选节点和连线（Delete）"));
    nodeRow->addStretch();
    auto *fitButton = addButton(nodeRow, QStringLiteral("适应画布"), false);
    auto *resetButton = addButton(nodeRow, QStringLiteral("100%"), false);
    resetButton->setToolTip(QStringLiteral("恢复 100% 视图缩放"));
    auto *openButton = addButton(fileRow, QStringLiteral("打开 JSON"), false);
    openButton->setToolTip(QStringLiteral("打开本地节点图（Ctrl+O）"));
    auto *saveButton = addButton(fileRow, QStringLiteral("保存 JSON"), true);
    saveButton->setToolTip(QStringLiteral("保存节点图，可选择保存位置（Ctrl+S）"));
    auto *exampleButton = addButton(fileRow, QStringLiteral("恢复示例"), false);
    auto *clearButton = addButton(fileRow, QStringLiteral("清空"), false);
    fileRow->addStretch();
    layout->addWidget(toolbar);

    auto *instructions = new QLabel(
        QStringLiteral("拖动端口连线；空白框选/Ctrl多选；Delete删除；双击节点改名；"
                       "滚轮缩放；中键/空格+左键平移；Esc取消"), this);
    instructions->setObjectName(QStringLiteral("NodeInstructions"));
    instructions->setWordWrap(true);
    layout->addWidget(instructions);

    m_editor = new NodeEditorWidget(this);
    m_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_editor, 1);

    auto *statusCard = new QFrame(this);
    statusCard->setObjectName(QStringLiteral("NodeStatusCard"));
    auto *statusLayout = new QVBoxLayout(statusCard);
    statusLayout->setContentsMargins(12, 9, 12, 9);
    statusLayout->setSpacing(5);
    auto *statisticsRow = new QHBoxLayout;
    statisticsRow->setSpacing(24);
    statusLayout->addLayout(statisticsRow);
    m_nodeCountLabel = new QLabel(statusCard);
    m_edgeCountLabel = new QLabel(statusCard);
    m_zoomLabel = new QLabel(statusCard);
    m_nodeCountLabel->setTextFormat(Qt::PlainText);
    m_edgeCountLabel->setTextFormat(Qt::PlainText);
    m_zoomLabel->setTextFormat(Qt::PlainText);
    statisticsRow->addWidget(m_nodeCountLabel);
    statisticsRow->addWidget(m_edgeCountLabel);
    statisticsRow->addWidget(m_zoomLabel);
    statisticsRow->addStretch();
    m_messageLabel = new QLabel(statusCard);
    m_messageLabel->setTextFormat(Qt::PlainText);
    m_messageLabel->setWordWrap(true);
    m_messageLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_messageLabel->setMinimumHeight(m_messageLabel->fontMetrics().height());
    statusLayout->addWidget(m_messageLabel);
    layout->addWidget(statusCard);

    connect(m_editor, &NodeEditorWidget::graphChanged, this, [this]() {
        updateStatistics();
        if (!m_loading) {
            m_dirty = true;
            updateTitle();
        }
    });
    connect(m_editor, &NodeEditorWidget::zoomChanged, this, &NodeEditorDemo::updateZoom);
    connect(m_editor, &NodeEditorWidget::message, this, [this](const QString &text) {
        setStatus(text);
    });

    const auto addNode = [this](NodeEditorWidget::Kind kind, const QString &name) {
        const QString id = m_editor->addNode(kind);
        if (id.isEmpty()) {
            setStatus(QStringLiteral("未能添加%1节点。").arg(name), true);
        } else {
            setStatus(QStringLiteral("已添加%1节点，可拖动端口创建连线。").arg(name));
        }
        m_editor->setFocus(Qt::OtherFocusReason);
    };
    connect(inputButton, &QPushButton::clicked, this, [addNode]() {
        addNode(NodeEditorWidget::Input, QStringLiteral("输入"));
    });
    connect(processButton, &QPushButton::clicked, this, [addNode]() {
        addNode(NodeEditorWidget::Process, QStringLiteral("处理"));
    });
    connect(outputButton, &QPushButton::clicked, this, [addNode]() {
        addNode(NodeEditorWidget::Output, QStringLiteral("输出"));
    });
    connect(deleteButton, &QPushButton::clicked, m_editor, &NodeEditorWidget::deleteSelected);
    connect(fitButton, &QPushButton::clicked, m_editor, &NodeEditorWidget::fitToGraph);
    connect(resetButton, &QPushButton::clicked, m_editor, &NodeEditorWidget::resetZoom);
    connect(openButton, &QPushButton::clicked, this, &NodeEditorDemo::openJson);
    connect(saveButton, &QPushButton::clicked, this, [this]() { saveJson(); });
    connect(exampleButton, &QPushButton::clicked, this, &NodeEditorDemo::restoreExample);
    connect(clearButton, &QPushButton::clicked, this, &NodeEditorDemo::clearGraph);

    auto *openShortcut = new QShortcut(QKeySequence(QKeySequence::Open), this);
    openShortcut->setContext(Qt::WindowShortcut);
    connect(openShortcut, &QShortcut::activated, this, &NodeEditorDemo::openJson);
    auto *saveShortcut = new QShortcut(QKeySequence(QKeySequence::Save), this);
    saveShortcut->setContext(Qt::WindowShortcut);
    connect(saveShortcut, &QShortcut::activated, this, [this]() { saveJson(); });

    updateStatistics();
    updateZoom(m_editor->zoomFactor());
    updateTitle();
    layout->activate();
    loadGraph(exampleGraph(), QString(), false,
              QStringLiteral("已载入四节点示例。拖动端口可编辑连线。"));
}

void NodeEditorDemo::openJson()
{
    const QString path = QFileDialog::getOpenFileName(
        this, QStringLiteral("打开节点图 JSON"),
        m_currentPath.isEmpty() ? documentsDirectory() : m_currentPath,
        QStringLiteral("节点图 JSON (*.json);;所有文件 (*)"));
    if (path.isEmpty())
        return;

    QFile file(path, this);
    if (!file.open(QIODevice::ReadOnly)) {
        showError(QStringLiteral("无法打开 JSON"),
                  QStringLiteral("无法读取文件：\n%1\n\n%2")
                      .arg(QDir::toNativeSeparators(path), file.errorString()));
        return;
    }
    const QByteArray data = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        showError(QStringLiteral("读取 JSON 失败"),
                  QStringLiteral("读取文件时发生错误，当前节点图已保留。\n%1")
                      .arg(file.errorString()));
        return;
    }
    file.close();

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        showError(QStringLiteral("JSON 格式错误"),
                  QStringLiteral("JSON 解析失败（字节位置 %1）：\n%2")
                      .arg(parseError.offset).arg(parseError.errorString()));
        return;
    }
    if (!document.isObject()) {
        showError(QStringLiteral("节点图格式错误"),
                  QStringLiteral("JSON 顶层必须是包含 version、nodes 和 edges 的对象。"));
        return;
    }
    if (!confirmUnsavedChanges())
        return;

    loadGraph(document.object(), QFileInfo(path).absoluteFilePath(), false,
              QStringLiteral("已打开 JSON：%1").arg(QDir::toNativeSeparators(path)));
}

bool NodeEditorDemo::saveJson()
{
    const QString initialPath = m_currentPath.isEmpty()
        ? QDir(documentsDirectory()).filePath(QStringLiteral("节点图.json"))
        : m_currentPath;
    QString path = QFileDialog::getSaveFileName(
        this, QStringLiteral("保存节点图 JSON"), initialPath,
        QStringLiteral("节点图 JSON (*.json)"), nullptr, QFileDialog::DontConfirmOverwrite);
    if (path.isEmpty())
        return false;

    const QString suffix = QFileInfo(path).suffix();
    if (suffix.isEmpty()) {
        path += QStringLiteral(".json");
    } else if (suffix.compare(QStringLiteral("json"), Qt::CaseInsensitive) != 0) {
        showError(QStringLiteral("不支持的文件后缀"),
                  QStringLiteral("节点图仅支持保存为 .json 文件，请重新选择文件名。"));
        return false;
    }
    path = QDir::cleanPath(QFileInfo(path).absoluteFilePath());
    const QFileInfo target(path);
    if (target.isDir()) {
        showError(QStringLiteral("无法保存 JSON"),
                  QStringLiteral("目标路径是文件夹，请选择 JSON 文件名。"));
        return false;
    }

    // 后缀补全后再确认，避免确认的文件与实际写入的目标不同。
    if (target.exists() && !sameFile(path, m_currentPath)) {
        QMessageBox prompt(QMessageBox::Question, QStringLiteral("确认覆盖文件"),
                           QStringLiteral("文件已存在：\n%1\n\n是否覆盖此文件？")
                               .arg(QDir::toNativeSeparators(path)),
                           QMessageBox::Save | QMessageBox::Cancel, this);
        prompt.setTextFormat(Qt::PlainText);
        prompt.button(QMessageBox::Save)->setText(QStringLiteral("覆盖"));
        prompt.button(QMessageBox::Cancel)->setText(QStringLiteral("取消"));
        prompt.setDefaultButton(QMessageBox::Cancel);
        prompt.setEscapeButton(QMessageBox::Cancel);
        if (prompt.exec() != QMessageBox::Save)
            return false;
    }

    const QByteArray data = QJsonDocument(m_editor->toJson()).toJson(QJsonDocument::Indented);
    QSaveFile file(path, this);
    if (!file.open(QIODevice::WriteOnly)) {
        showError(QStringLiteral("无法保存 JSON"),
                  QStringLiteral("无法写入文件：\n%1\n\n%2")
                      .arg(QDir::toNativeSeparators(path), file.errorString()));
        return false;
    }
    if (file.write(data) != static_cast<qint64>(data.size())) {
        const QString error = file.errorString();
        file.cancelWriting();
        showError(QStringLiteral("保存 JSON 失败"),
                  QStringLiteral("文件未能完整写入，修改仍未保存。\n%1").arg(error));
        return false;
    }
    if (!file.commit()) {
        showError(QStringLiteral("保存 JSON 失败"),
                  QStringLiteral("无法完成文件保存，修改仍未保存。\n%1").arg(file.errorString()));
        return false;
    }

    m_currentPath = path;
    m_dirty = false;
    updateTitle();
    setStatus(QStringLiteral("已保存 JSON：%1").arg(QDir::toNativeSeparators(path)));
    return true;
}

bool NodeEditorDemo::confirmUnsavedChanges()
{
    if (!m_dirty)
        return true;

    const QString name = m_currentPath.isEmpty()
        ? QStringLiteral("未命名节点图") : QFileInfo(m_currentPath).fileName();
    QMessageBox prompt(QMessageBox::Question, QStringLiteral("保存修改"),
                       QStringLiteral("“%1”有未保存的修改。\n是否先保存再继续？").arg(name),
                       QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, this);
    prompt.setTextFormat(Qt::PlainText);
    prompt.button(QMessageBox::Save)->setText(QStringLiteral("保存"));
    prompt.button(QMessageBox::Discard)->setText(QStringLiteral("放弃"));
    prompt.button(QMessageBox::Cancel)->setText(QStringLiteral("取消"));
    prompt.setDefaultButton(QMessageBox::Save);
    prompt.setEscapeButton(QMessageBox::Cancel);
    const int choice = prompt.exec();
    if (choice == QMessageBox::Save)
        return saveJson();
    return choice == QMessageBox::Discard;
}

void NodeEditorDemo::restoreExample()
{
    if (!confirmUnsavedChanges())
        return;
    loadGraph(exampleGraph(), m_currentPath, true, QStringLiteral("已恢复四节点示例，修改尚未保存。"));
}

void NodeEditorDemo::clearGraph()
{
    if (!confirmUnsavedChanges())
        return;
    loadGraph(emptyGraph(), m_currentPath, true, QStringLiteral("已清空节点图，修改尚未保存。"));
}

bool NodeEditorDemo::loadGraph(const QJsonObject &object, const QString &path,
                               bool modified, const QString &successMessage)
{
    QString error;
    bool loaded = false;
    {
        QScopedValueRollback<bool> loading(m_loading, true);
        loaded = m_editor->fromJson(object, &error);
    }
    if (!loaded) {
        if (error.isEmpty())
            error = QStringLiteral("请检查版本号、节点、端口及连线是否符合节点图格式。");
        showError(QStringLiteral("无法载入节点图"),
                  QStringLiteral("节点图内容无效，当前图已保留。\n%1").arg(error));
        return false;
    }

    m_currentPath = path;
    m_dirty = modified;
    updateStatistics();
    updateTitle();
    m_editor->fitToGraph();
    updateZoom(m_editor->zoomFactor());
    m_editor->setFocus(Qt::OtherFocusReason);
    setStatus(successMessage);
    return true;
}

void NodeEditorDemo::updateStatistics()
{
    m_nodeCountLabel->setText(QStringLiteral("节点数：%1").arg(m_editor->nodeCount()));
    m_edgeCountLabel->setText(QStringLiteral("连线数：%1").arg(m_editor->edgeCount()));
}

void NodeEditorDemo::updateZoom(qreal factor)
{
    m_zoomLabel->setText(QStringLiteral("缩放：%1%").arg(factor * 100.0, 0, 'f', 0));
}

void NodeEditorDemo::updateTitle()
{
    const QString name = m_currentPath.isEmpty()
        ? QStringLiteral("未命名节点图") : QFileInfo(m_currentPath).fileName();
    setWindowTitle(QStringLiteral("%1%2 · NodeEditor 节点编辑器")
                       .arg(name, m_dirty ? QStringLiteral(" *") : QString()));
}

void NodeEditorDemo::setStatus(const QString &text, bool error)
{
    m_messageLabel->setText(text);
    m_messageLabel->setStyleSheet(error
        ? QStringLiteral("color: #B91C1C;") : QStringLiteral("color: #64748B;"));
}

void NodeEditorDemo::showError(const QString &title, const QString &text)
{
    setStatus(text, true);
    QMessageBox dialog(QMessageBox::Warning, title, text, QMessageBox::Ok, this);
    dialog.setTextFormat(Qt::PlainText);
    dialog.button(QMessageBox::Ok)->setText(QStringLiteral("确定"));
    dialog.exec();
}

void NodeEditorDemo::closeEvent(QCloseEvent *event)
{
    if (confirmUnsavedChanges())
        event->accept();
    else
        event->ignore();
}

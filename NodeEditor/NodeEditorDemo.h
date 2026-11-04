#pragma once

#include <QJsonObject>
#include <QString>
#include <QWidget>

class NodeEditorWidget;
class QCloseEvent;
class QLabel;

class NodeEditorDemo : public QWidget
{
public:
    explicit NodeEditorDemo(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void openJson();
    bool saveJson();
    bool confirmUnsavedChanges();
    void restoreExample();
    void clearGraph();
    bool loadGraph(const QJsonObject &object, const QString &path,
                   bool modified, const QString &successMessage);
    void updateStatistics();
    void updateZoom(qreal factor);
    void updateTitle();
    void setStatus(const QString &text, bool error = false);
    void showError(const QString &title, const QString &text);

    NodeEditorWidget *m_editor = nullptr;
    QLabel *m_nodeCountLabel = nullptr;
    QLabel *m_edgeCountLabel = nullptr;
    QLabel *m_zoomLabel = nullptr;
    QLabel *m_messageLabel = nullptr;
    QString m_currentPath;
    bool m_dirty = false;
    bool m_loading = false;
};

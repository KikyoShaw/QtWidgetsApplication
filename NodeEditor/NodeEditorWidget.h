#pragma once

#include <QWidget>
#include <QJsonObject>
#include <QString>

#include <memory>

class NodeEditorWidget : public QWidget
{
    Q_OBJECT

public:
    enum Kind { Input, Process, Output };
    Q_ENUM(Kind)

    explicit NodeEditorWidget(QWidget *parent = nullptr);
    ~NodeEditorWidget() override;

    QString addNode(Kind kind);
    int nodeCount() const;
    int edgeCount() const;
    qreal zoomFactor() const;
    QJsonObject toJson() const;
    bool fromJson(const QJsonObject &object, QString *error = nullptr);

public slots:
    void deleteSelected();
    void selectAll();
    void fitToGraph();
    void resetZoom();

signals:
    void graphChanged();
    void zoomChanged(qreal factor);
    void message(const QString &text);

private:
    class Private;
    std::unique_ptr<Private> d;
};
#pragma once

#include <QColor>
#include <QWidget>
#include <memory>

class ColorPickerWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor selectedColor READ selectedColor WRITE setSelectedColor NOTIFY colorChanged)

public:
    explicit ColorPickerWidget(QWidget *parent = nullptr);
    ~ColorPickerWidget() override;

    QColor selectedColor() const;

public slots:
    void setSelectedColor(const QColor &color);

signals:
    void colorChanged(const QColor &color);

private:
    struct Private;
    std::unique_ptr<Private> d;
};
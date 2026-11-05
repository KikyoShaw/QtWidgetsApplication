#pragma once

#include <QWidget>

class QColor;
class QCheckBox;
class QComboBox;
class QEvent;
class QHideEvent;
class QLabel;
class QLineEdit;
class QPushButton;
class QShowEvent;
class QSlider;
class QSpinBox;
class QTimer;
class DanmakuWidget;

class DanmakuDemo : public QWidget
{
public:
    explicit DanmakuDemo(QWidget *parent = nullptr);
    ~DanmakuDemo() override;

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void changeEvent(QEvent *event) override;

private:
    void syncSettings();
    void syncStatistics();
    void updateSampleTimer();
    bool canGenerateSamples() const;
    void sendSample();
    void sendInput();
    QColor inputColor() const;

    DanmakuWidget *m_danmaku = nullptr;
    QSlider *m_fontSlider = nullptr;
    QSpinBox *m_fontSpin = nullptr;
    QSpinBox *m_laneSpin = nullptr;
    QSlider *m_opacitySlider = nullptr;
    QLabel *m_opacityValue = nullptr;
    QSlider *m_speedSlider = nullptr;
    QSpinBox *m_speedSpin = nullptr;
    QPushButton *m_pauseButton = nullptr;
    QCheckBox *m_autoSamples = nullptr;
    QLabel *m_statistics = nullptr;
    QLabel *m_laneHint = nullptr;
    QLineEdit *m_input = nullptr;
    QComboBox *m_colorCombo = nullptr;
    QTimer *m_sampleTimer = nullptr;
    bool m_displayed = false;
    bool m_seeded = false;
};
#ifndef INTERACTIVEBUTTONBASE_H
#define INTERACTIVEBUTTONBASE_H

#include <QObject>
#include <QPushButton>
#include <QPoint>
#include <QTimer>
#include <QPropertyAnimation>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QDateTime>
#include <QList>
#include <QBitmap>
#include <QtMath>

#define PI 3.1415926
#define GOLDEN_RATIO 0.618

#define DOUBLE_PRESS_INTERVAL 200 // �ɿ��Ͱ��µļ�������Ϊ˫��
#define SINGLE_PRESS_INTERVAL 150 // ����ʱ�䳬����������ǵ��������Ϊ����

/**
 * Copyright (c) 2019 ��ȼо�V All rights reserved.
 ��
 * ���䣺iwxyiii@gmail.com
 * QQ�ţ�482582886
 * ʱ�䣺2019.11.1
 *
 * ˵�������Ե��Զ��尴ť��������ȥ
 * Դ�룺https://github.com/MRXY001/Interactive-Windows-Buttons
 *
 * ������Ϊ���˱�д�����Լ�ʹ�ã�������˽�͸�������ʹ�á�
 * �����Ȩ���������У�ֻ��ʹ�ò��ܳ��ۣ�Υ���߱�����Ȩ׷�����Ρ�
 */

class InteractiveButtonBase : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(bool self_enabled WRITE setSelfEnabled)                // �Ƿ������Զ���İ�ť��true��
    Q_PROPERTY(bool parent_enabled WRITE setParentEnabled)            // �Ƿ����ø��ఴť��false��
    Q_PROPERTY(bool fore_enabled WRITE setForeEnabled)                // �Ƿ�����Զ��尴ťǰ��ɫ��true��
    Q_PROPERTY(QString text WRITE setText)                            // ǰ������
    Q_PROPERTY(QString icon_path WRITE setIconPath)                   // ǰ��ͼ��
    Q_PROPERTY(QString pixmpa_path WRITE setPixmapPath)               // ǰ��ͼ��
    Q_PROPERTY(QColor icon_color WRITE setIconColor)                  // ǰ��ͼ��˧ɫ
    Q_PROPERTY(QColor text_color WRITE setTextColor)                  // ǰ��������ɫ
    Q_PROPERTY(QColor background_color WRITE setNormalColor)          // ������ɫ
    Q_PROPERTY(QColor border_color WRITE setBorderColor)              // �߽���ɫ
    Q_PROPERTY(QColor hover_color WRITE setHoverColor)                // �������������ɫ
    Q_PROPERTY(QColor press_color WRITE setPressColor)                // ��갴�±�����ɫ
    Q_PROPERTY(int hover_duration WRITE setHoverAniDuration)          // ���������������
    Q_PROPERTY(int press_duration WRITE setPressAniDuration)          // ��갴�¶�������
    Q_PROPERTY(int click_duration WRITE setClickAniDuration)          // �������������
    Q_PROPERTY(double icon_padding_proper WRITE setIconPaddingProper) // ͼ���ı߿հ״���С����
    Q_PROPERTY(int radius WRITE setRadius)                            // �߿�Բ�ǰ뾶
    Q_PROPERTY(int border_width WRITE setBorderWidth)                 // �߿�������ϸ
    Q_PROPERTY(bool fixed_fore_pos WRITE setFixedTextPos)             // �Ƿ�̶�ǰ��λ�ã�false��
    Q_PROPERTY(bool text_dynamic_size WRITE setTextDynamicSize)       // �޸������Сʱ������ť��С�ߴ磨false��
    Q_PROPERTY(bool leave_after_clicked WRITE setLeaveAfterClick)     // ��굥���ɿ���ȡ������Ч������Բ˵���������
    Q_PROPERTY(bool show_animation WRITE setShowAni)                  // �Ƿ����ó��ֶ���������ƿ�����ʧ����false��
    Q_PROPERTY(bool water_animation WRITE setWaterRipple)             // �Ƿ����õ��ˮ���ƶ���������ʹ�ý��䣩��true��
    Q_PROPERTY(int font_size READ getFontSizeT WRITE setFontSizeT)    // ������ť���嶯��Ч�����Զ�����Ӧ�����ã�
public:
    InteractiveButtonBase(QWidget *parent = nullptr);
    InteractiveButtonBase(QString text, QWidget *parent = nullptr);
    InteractiveButtonBase(QIcon icon, QWidget *parent = nullptr);
    InteractiveButtonBase(QPixmap pixmap, QWidget *parent = nullptr);

    /**
     * ǰ��ʵ��
     */
    enum PaintModel
    {
        None,       // ��ǰ������ʹ�ñ���
        Text,       // �����֣�������ࣩ
        Icon,       // ��ͼ��
        PixmapMask, // �ɱ�ɫͼ�꣨ͨ��pixmap+����ʵ�֣�����ݻ�����
        IconText,   // ͼ��+���֣��ݲ�֧�֣�
        PixmapText  // ��ɫͼ��+���֣��ݲ�֧�֣�
    };

    /**
     * ǰ�������ͼ�꣨���Զ����
     * �����ǽǱ꣨����չ����ͷ��
     * ����ʱǰ׺��ͼ����
     */
    struct PaintAddin
    {
        PaintAddin() : enable(false) {}
        PaintAddin(QPixmap p, Qt::Alignment a, QSize s) : enable(true), pixmap(p), align(a), size(s) {}
        bool enable;         // �Ƿ�����
        QPixmap pixmap;      // �ɱ�ɫͼ��
        Qt::Alignment align; // ���뷽ʽ
        QSize size;          // �̶���С
    };

    /**
     * ����ɿ�ʱ��������
     * �ɿ���ʱ�����ÿһ�ζ�������+ʱ�䣬���������
     * ��ʱ���������Ķ���ʵ������
     */
    struct Jitter
    {
        Jitter(QPoint p, qint64 t) : point(p), timestamp(t) {}
        QPoint point;     // Ҫ�˶�����Ŀ������
        qint64 timestamp; // �˶���Ŀ������Ӧ�õ�ʱ�����������ɾ�����ζ���·������
    };

    /**
     * ��갴��/����ˮ���ƶ���
     * ��갴��ʱ�����ٶ�����ѹס�����ɿ��󶯻��ٶ���Ȼ�ӿ�
     * ͬ���ö��м�¼���е�ˮ���ƶ���ʵ��
     */
    struct Water
    {
        Water(QPoint p, qint64 t) : point(p), progress(0), press_timestamp(t),
                                    release_timestamp(0), finish_timestamp(0), finished(false) {}
        QPoint point;
        int progress;             // ˮ���ƽ���100%�������ã���ǰʹ��ʱ�����
        qint64 press_timestamp;   // ��갴��ʱ���
        qint64 release_timestamp; // ����ɿ�ʱ������밴��ʱ���������ʱ���һ���Ϊˮ���ƽ��ȼ������
        qint64 finish_timestamp;  // ����ʱ������뵱ǰʱ��������Ϊ������ʧ������ʱ���
        bool finished;            // �Ƿ�������������Ϊ������ʧ
    };

    /**
     * ���ܱ߽��padding
     * ������ť��Сʱ�����+���ҡ��߶�+����
     */
    struct EdgeVal
    {
        EdgeVal() {}
        EdgeVal(int l, int t, int r, int b) : left(l), top(t), right(r), bottom(b) {}
        int left, top, right, bottom; // �ĸ��߽�Ŀհ׾���
    };

    enum NolinearType {
        Linear,
        SlowFaster,
        FastSlower,
        SlowFastSlower,
        SpringBack20,
        SpringBack50
    };

    virtual void setText(QString text);
    virtual void setIconPath(QString path);
    virtual void setIcon(QIcon icon);
    virtual void setPixmapPath(QString path);
    virtual void setPixmap(QPixmap pixmap);
    virtual void setPaintAddin(QPixmap pixmap, Qt::Alignment align = Qt::AlignRight, QSize size = QSize(0, 0));

    void setSelfEnabled(bool e = true);
    void setParentEnabled(bool e = false);
    void setForeEnabled(bool e = true);

    void setHoverAniDuration(int d);
    void setPressAniDuration(int d);
    void setClickAniDuration(int d);
    void setWaterAniDuration(int press, int release, int finish);
    void setWaterRipple(bool enable = true);
    void setJitterAni(bool enable = true);
    void setUnifyGeomerey(bool enable = true);
    void setBgColor(QColor bg);
    void setBgColor(QColor hover, QColor press);
    void setNormalColor(QColor color);
    void setBorderColor(QColor color);
    void setHoverColor(QColor color);
    void setPressColor(QColor color);
    void setIconColor(QColor color = QColor(0, 0, 0));
    void setTextColor(QColor color = QColor(0, 0, 0));
    void setFontSize(int f);
    void setHover();
    void setAlign(Qt::Alignment a);
    void setRadius(int r);
    void setRadius(int rx, int ry);
    void setBorderWidth(int x);
    void setDisabled(bool dis = true);
    void setPaddings(int l, int r, int t, int b);
    void setPaddings(int h, int v);
    void setPaddings(int x);
    void setIconPaddingProper(double x);
    void setFixedForePos(bool f = true);
    void setFixedForeSize(bool f = true, int addin = 0);
    void setSquareSize();
    void setTextDynamicSize(bool d = true);
    void setLeaveAfterClick(bool l = true);
    void setDoubleClicked(bool e = true);

    void setShowAni(bool enable = true);
    void showForeground();
    void showForeground2(QPoint point = QPoint(0, 0));
    void hideForeground();
    void delayShowed(int time, QPoint point = QPoint(0, 0));

    void setMenu(QMenu *menu);
    void setState(bool s = true);
    bool getState();
    virtual void simulateStatePress(bool s = true);

#if QT_DEPRECATED_SINCE(5, 11)
    QT_DEPRECATED_X("Use InteractiveButtonBase::setFixedForePos(bool fixed = true)")
    void setFixedTextPos(bool f = true);
#endif

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;
    void changeEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    virtual bool inArea(QPoint point);
    virtual QPainterPath getBgPainterPath();
    virtual QPainterPath getWaterPainterPath(Water water);

    QRect getUnifiedGeometry();
    void updateUnifiedGeometry();
    void paintWaterRipple(QPainter &painter);
    void setJitter();

    int getFontSizeT();
    void setFontSizeT(int f);

    int max(int a, int b) const;
    int min(int a, int b) const;
    int quick_sqrt(long X) const;
    qint64 getTimestamp() const;
    bool isLightColor(QColor color);
    int getSpringBackProgress(int x, int max);
    QColor getOpacityColor(QColor color, double level = 0.5);
    QPixmap getMaskPixmap(QPixmap p, QColor c);

    double getNolinearProg(int p, NolinearType type);

signals:
    void showAniFinished();
    void hideAniFinished();
    void pressAppearAniFinished();
    void pressDisappearAniFinished();
    void jitterAniFinished();
    void doubleClicked();

public slots:
    virtual void anchorTimeOut();
    virtual void slotClicked();
    void slotCloseState();

public:
    PaintModel model;
    QIcon icon;
    QString text;
    QPixmap pixmap;
    PaintAddin paint_addin;
    EdgeVal icon_paddings;

protected:
    // ���忪��
    bool self_enabled, parent_enabled, fore_enabled; // �Ƿ��������ࡢ�������ࡢ��������ǰ��

    // ����ǰ���Ķ���
    bool show_animation, show_foreground;
    bool show_ani_appearing, show_ani_disappearing;
    int show_duration;
    qint64 show_timestamp, hide_timestamp;
    int show_ani_progress;
    QPoint show_ani_point;

    // ��꿪ʼ���������¡��ɿ����뿪�������ʱ���
    // ���ê�㡢Ŀ��ê�㡢��ǰê������ꣻ��ǰXY��ƫ����
    QPoint enter_pos, press_pos, release_pos, mouse_pos, anchor_pos /*Ŀ��ê�㽥���������*/;
    QPoint offset_pos /*��ǰƫ����*/, effect_pos, release_offset;                // ������ġ�������Ͻǡ�����ʱ��ƽ����ƫ��
    bool hovering, pressing;                                                     // �Ƿ������Ͱ��µ�״̬��
    qint64 hover_timestamp, leave_timestamp, press_timestamp, release_timestamp; // �����¼���ʱ���
    int hover_bg_duration, press_bg_duration, click_ani_duration;                // ���ֶ���ʱ��

    // ��ʱˢ�½��棨��֤����������
    QTimer *anchor_timer;
    int move_speed;

    // ������ǰ��
    QColor icon_color, text_color;                   // ǰ����ɫ
    QColor normal_bg, hover_bg, press_bg, border_bg; // ���ֱ�����ɫ
    int hover_speed, press_start, press_speed;       // ��ɫ�����ٶ�
    int hover_progress, press_progress;              // ��ɫ�������
    double icon_padding_proper;                      // ͼ��Ĵ�С����
    int border_width;
    int radius_x, radius_y;
    int font_size;
    bool fixed_fore_pos;    // ������ʱ�Ƿ�̶�����λ��
    bool fixed_fore_size;   // ������/���ʱ�Ƿ�̶�ǰ����С
    bool text_dynamic_size; // ��������ʱ�Զ�������С���

    // ��굥������
    bool click_ani_appearing, click_ani_disappearing; // �Ƿ����ڰ��µĶ���Ч����
    int click_ani_progress;                           // ���µĽ��ȣ�ʹ��ʱ�����㣩

    // ͳһ����ͼ������򣨴�������ť��Ϊ��������֮�������Ҹ���ƫ�Ƽ��㣩
    bool unified_geometry; // �����ò����Ļ������Ҳ�ò�������
    int _l, _t, _w, _h;

    // �����ק�������ض���Ч��
    bool jitter_animation;      // �Ƿ�������ɿ�ʱ�Ķ���Ч��
    double elastic_coefficient; // ����ϵ��
    QList<Jitter> jitters;
    int jitter_duration; // ����һ�Σ����Ч������

    // ��갴��ˮ���ƶ���Ч��
    bool water_animation; // �Ƿ���ˮ���ƶ���
    QList<Water> waters;
    int water_press_duration, water_release_duration, water_finish_duration;
    int water_radius;

    // ����Ч��
    Qt::Alignment align;      // ����/ͼ����䷽��
    bool _state;              // һ����¼״̬�ı����������Ƿ����
    bool leave_after_clicked; // ��굥���ɿ���ȡ������Ч������Բ˵�������������ť�ض�ʧȥ����

    // ˫��
    bool double_clicked;      // ����˫��
    QTimer* double_timer;     // ˫��ʱ��
    bool double_prevent;      // ˫����ֹ����release��flag
};

#endif // INTERACTIVEBUTTONBASE_H

#include "interactivebuttonbase.h"

/**
 * �������ݵĳ�ʼ��
 * ���Ҫ�Զ��壬����������������е�Ĭ��ֵ
 */
InteractiveButtonBase::InteractiveButtonBase(QWidget *parent)
    : QPushButton(parent), icon(nullptr), text(""), paint_addin(),
      icon_paddings(4,4,4,4),
      self_enabled(true), parent_enabled(false), fore_enabled(true),
      show_animation(false), show_foreground(true), show_ani_appearing(false), show_ani_disappearing(false),
      show_duration(300), show_timestamp(0), hide_timestamp(0), show_ani_progress(0), show_ani_point(0,0),
      enter_pos(-1, -1), press_pos(-1, -1), release_pos(-1, -1), mouse_pos(-1, -1), anchor_pos(-1,  -1),
      offset_pos(0, 0), effect_pos(-1, -1), release_offset(0, 0),
      hovering(false), pressing(false),
      hover_timestamp(0), leave_timestamp(0), press_timestamp(0), release_timestamp(0),
      hover_bg_duration(300), press_bg_duration(300), click_ani_duration(300),
      move_speed(5),
      icon_color(0, 0, 0), text_color(0,0,0),
      normal_bg(0xF2, 0xF2, 0xF2, 0), hover_bg(128, 128, 128, 32), press_bg(128, 128, 128, 64), border_bg(0,0,0,0),
      hover_speed(5), press_start(40), press_speed(5),
      hover_progress(0), press_progress(0), icon_padding_proper(0.25),
      border_width(1), radius_x(0), radius_y(0), font_size(0), fixed_fore_pos(false), fixed_fore_size(false), text_dynamic_size(false),
      click_ani_appearing(false), click_ani_disappearing(false), click_ani_progress(0),
      unified_geometry(false), _l(0), _t(0), _w(32), _h(32),
      jitter_animation(true), elastic_coefficient(1.2), jitter_duration(300),
      water_animation(true), water_press_duration(800), water_release_duration(400), water_finish_duration(300),
      align(Qt::AlignCenter), _state(false), leave_after_clicked(false),
      double_clicked(false), double_timer(nullptr), double_prevent(false)
{
    setMouseTracking(true); // ���û�а���ʱҲ�ܲ����ƶ��¼�

    model = PaintModel::None;

    anchor_timer = new QTimer(this);
    anchor_timer->setInterval(10);
    connect(anchor_timer, SIGNAL(timeout()), this, SLOT(anchorTimeOut()));

    setWaterRipple();

    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()));

    setFocusPolicy(Qt::NoFocus); // ����һ����ť����ȡTab������
}

/**
 * �������͵İ�ť
 */
InteractiveButtonBase::InteractiveButtonBase(QString text, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    setText(text);
}

/**
 * ͼ�����͵İ�ť
 */
InteractiveButtonBase::InteractiveButtonBase(QIcon icon, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    setIcon(icon);
}

/**
 * ��ɫͼ�����͵İ�ť
 */
InteractiveButtonBase::InteractiveButtonBase(QPixmap pixmap, QWidget *parent)
    : InteractiveButtonBase(parent)
{
    setPixmap(pixmap);
}

/**
 * ���ð�ť����
 * @param text ��ť����
 */
void InteractiveButtonBase::setText(QString text)
{
    this->text = text;
    if (model == PaintModel::None) model = PaintModel::Text;
    if (parent_enabled)
        QPushButton::setText(text);

    // �������������С
    if (text_dynamic_size)
    {
        if (font_size <= 0)
        {
            QFontMetrics fm(font());
            setMinimumSize(fm.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right, fm.lineSpacing()+icon_paddings.top+icon_paddings.bottom);
        }
        else
        {
            QFont font;
            font.setPointSize(font_size);
            QFontMetrics fm(font);
            setMinimumSize(fm.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right, fm.lineSpacing()+icon_paddings.top+icon_paddings.bottom);
        }
    }
    update();
}

/**
 * ���� icon ͼ��
 * @param path ͼ��·���ı�
 */
void InteractiveButtonBase::setIconPath(QString path)
{
    setIcon(QIcon(path));
}

/**
 * ���� pixmap ͼ��
 * @param path ͼ��·���ı�
 */
void InteractiveButtonBase::setPixmapPath(QString path)
{
    setPixmap(QPixmap(path));
}

/**
 * ���� icon
 * @param icon ͼ��
 */
void InteractiveButtonBase::setIcon(QIcon icon)
{
    if (model == PaintModel::None)
        model = PaintModel::Icon;
    this->icon = icon;
    if (parent_enabled)
        QPushButton::setIcon(icon);
    update();
}

/**
 * ���� Pixmap
 * @param pixmap [description]
 */
void InteractiveButtonBase::setPixmap(QPixmap pixmap)
{
    if (model == PaintModel::None)
        model = PaintModel::PixmapMask;
    this->pixmap = getMaskPixmap(pixmap, isEnabled()?icon_color:getOpacityColor(icon_color));
    if (parent_enabled)
        QPushButton::setIcon(QIcon(pixmap));
    update();
}

/**
 * ���ö����ͼ�꣬����Ǳ�
 * @param pixmap ͼ��
 * @param align  ���뷽ʽ
 * @param size   ͼ��ߴ�
 */
void InteractiveButtonBase::setPaintAddin(QPixmap pixmap, Qt::Alignment align, QSize size)
{
    QBitmap mask = pixmap.mask();
    pixmap.fill(icon_color);
    pixmap.setMask(mask);
    paint_addin = PaintAddin(pixmap, align, size);
    update();
}

/**
 * �������๦���Ƿ���
 * ����رգ����൱��Ĭ�ϵ� QPushButton
 * @param e ����
 */
void InteractiveButtonBase::setSelfEnabled(bool e)
{
    self_enabled = e;
}

/**
 * ���ø��ࣨQPushButton�������Ƿ���
 * �������������Ƹ��౳��������ǰ��
 * @param e ����
 */
void InteractiveButtonBase::setParentEnabled(bool e)
{
    parent_enabled = e;

    // �����������ݵ�����ȥ����������ص�����ʾ
    if (model == PaintModel::Text || model == PaintModel::IconText || model == PaintModel::PixmapText)
        QPushButton::setText(text);
    if (model == PaintModel::Icon || model == PaintModel::IconText)
        QPushButton::setIcon(icon);
    if (model == PaintModel::PixmapMask || model == PaintModel::PixmapText)
        QPushButton::setIcon(QIcon(pixmap));
}

/**
 * �����Ƿ����ǰ��ͼ��/����
 * �رպ���ֻ���Ʊ���
 * @param e ����
 */
void InteractiveButtonBase::setForeEnabled(bool e)
{
    fore_enabled = e;
}

/**
 * �������������������Ķ���ʱ��
 * @param d ����ʱ�������룩
 */
void InteractiveButtonBase::setHoverAniDuration(int d)
{
    this->hover_bg_duration = d;
//    hover_progress = 0; // ����hoverЧ��
}

/**
 * ������갴�½���Ч���Ķ���ʱ��
 * @param d ����ʱ�������룩
 */
void InteractiveButtonBase::setPressAniDuration(int d)
{
    this->press_bg_duration = d;
}

/**
 * ���õ���Ч���Ķ���ʱ��
 * @param d ����ʱ�������룩
 */
void InteractiveButtonBase::setClickAniDuration(int d)
{
    this->click_ani_duration = d;
}

/**
 * ����ˮ���ƶ���ʱ��
 * @param press   ��סʱʱ����ʱ�����룩
 * @param release �ɿ����ٶȣ�ʱ�����룩
 * @param finish  ������ʧ�ٶȣ�ʱ�����룩
 */
void InteractiveButtonBase::setWaterAniDuration(int press, int release, int finish)
{
    this->water_press_duration = press;
    this->water_release_duration = release;
    this->water_finish_duration = finish;
}

/**
 * ����״̬�ı�
 * ��Ҫ�Ǽ�� ���� ״̬��������ʱ����Ϊ��͸��
 */
void InteractiveButtonBase::changeEvent(QEvent *event)
{
    QPushButton::changeEvent(event);

    if (event->type() == QEvent::EnabledChange && model == PixmapMask) // ����״̬�ı���
    {
        if (isEnabled()) // �ָ����ã�͸���ȱ��ȥ
        {
            QColor color = icon_color;
            color.setAlpha(color.alpha() * 2);
            setIconColor(color);
        }
        else // ��ɲ����ã�͸���ȼ���
        {
            QColor color = icon_color;
            color.setAlpha(color.alpha() / 2);
            setIconColor(color);
        }
    }
}

/**
 * ����ˮ���ƶ����Ƿ���
 * �ر�ʱ����ʹ�ý��䶯��
 * @param enable ����
 */
void InteractiveButtonBase::setWaterRipple(bool enable)
{
    if (water_animation == enable) return ;
    water_animation = enable;
}

/**
 * ���ö���Ч���Ƿ���
 * �����ק�ƶ��ľ���Խ������������Խ��������Խ��
 * @param enable ����
 */
void InteractiveButtonBase::setJitterAni(bool enable)
{
    jitter_animation = enable;
}

/**
 * �����Ƿ�ʹ��ͳһͼ���������
 * ����ͼ��ߴ��С�仯�����ĵ�ƫ�ƣ������µ���������λ��
 * @param enable ����
 */
void InteractiveButtonBase::setUnifyGeomerey(bool enable)
{
    unified_geometry = enable;
    _l = _t = 0; _w = size().width(); _h = size().height();
}

/**
 * ���ñ�����ɫ
 * @param bg ������ɫ
 */
void InteractiveButtonBase::setBgColor(QColor bg)
{
    setNormalColor(bg);
    update();
}

/**
 * �����¼�������ɫ
 * @param hover �������ʱ�ı�����ɫ
 * @param press ��갴��ʱ�ı�����ɫ
 */
void InteractiveButtonBase::setBgColor(QColor hover, QColor press)
{
    if (hover != Qt::black)
        setHoverColor(hover);
    if (press != Qt::black)
        setPressColor(press);
    update();
}

/**
 * ���ð�ť������ɫ
 * @param color ������ɫ
 */
void InteractiveButtonBase::setNormalColor(QColor color)
{
    normal_bg = color;
}

/**
 * ���ñ߿�������ɫ
 * @param color �߿���ɫ
 */
void InteractiveButtonBase::setBorderColor(QColor color)
{
    border_bg = color;
}

/**
 * �����������ʱ�ı�����ɫ
 * @param color ������ɫ
 */
void InteractiveButtonBase::setHoverColor(QColor color)
{
    hover_bg = color;
}

/**
 * ������갴סʱ�ı�����ɫ
 * @param color ������ɫ
 */
void InteractiveButtonBase::setPressColor(QColor color)
{
    press_bg = color;
}

/**
 * ����ͼ����ɫ������Կɱ�ɫ�� pixmap ͼ�꣩
 * @param color ͼ����ɫ
 */
void InteractiveButtonBase::setIconColor(QColor color)
{
    icon_color = color;

    // ����ͼ�꣨����У�
    if (model == PaintModel::PixmapMask || model == PaintModel::PixmapText)
    {
        pixmap = getMaskPixmap(pixmap, isEnabled()?icon_color:getOpacityColor(icon_color));
    }

    // ���ƶ���Ǳ꣨����еĻ���
    if (paint_addin.enable)
    {
        paint_addin.pixmap = getMaskPixmap(paint_addin.pixmap, isEnabled()?icon_color:getOpacityColor(icon_color));
    }

    update();
}

/**
 * ����ǰ��������ɫ
 * @param color ������ɫ
 */
void InteractiveButtonBase::setTextColor(QColor color)
{
    text_color = color;
    update();
}

/**
 * �������ִ�С��PointSize������ font() �����С��
 * @param f ���ִ�С
 */
void InteractiveButtonBase::setFontSize(int f)
{
    if (!font_size) // ��һ�����������С��ֱ������
    {
        font_size = f;
        QFont font(this->font());
        font.setPointSize(f);
        setFont(font);
        update();
    }
    else // �ı������С��ʹ���������Ŷ���
    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "font_size");
        ani->setStartValue(font_size);
        ani->setEndValue(f);
        ani->setDuration(click_ani_duration);
        connect(ani, &QPropertyAnimation::finished, [=]{
            ani->deleteLater();
        });
        ani->start();
    }
    // �޸������Сʱ������ť����С�ߴ磬����������ʾ��ȫ
    if (text_dynamic_size)
    {
        QFont font;
        font.setPointSize(f);
        QFontMetrics fms(font);
        setMinimumSize(fms.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right, fms.lineSpacing()+icon_paddings.top+icon_paddings.bottom);
    }
}

/**
 * ��ȡ�����С
 * ������Ϊ���嶯�������Բ���
 * @return ��ʱ�����С
 */
int InteractiveButtonBase::getFontSizeT()
{
    return font_size;
}

/**
 * ���ö����е���ʱ�����С
 * ������Ϊ���嶯�������Բ���
 * @param f ��ʱ�����С
 */
void InteractiveButtonBase::setFontSizeT(int f)
{
    this->font_size = f;
    QFont font(this->font());
    font.setPointSize(f);
    setFont(font);
    update();
}

/**
 * ������ʧȥ����Ļ�����ʹ����Ƶ����棬Ҳ������ֱ���
 * �������������������������״̬
 */
void InteractiveButtonBase::setHover()
{
    if (!hovering && inArea(mapFromGlobal(QCursor::pos())))
        InteractiveButtonBase::enterEvent(new QEvent(QEvent::Type::None));
}

/**
 * ���ö��뷽ʽ
 * @param a ���뷽ʽ
 */
void InteractiveButtonBase::setAlign(Qt::Alignment a)
{
    align = a;
    update();
}

/**
 * �����ĸ��ǵİ뾶
 * @param r �뾶
 */
void InteractiveButtonBase::setRadius(int r)
{
    radius_x = radius_y = r;
}

/**
 * �ֿ����� X��Y �İ뾶
 * @param rx X�뾶
 * @param ry Y�뾶
 */
void InteractiveButtonBase::setRadius(int rx, int ry)
{
    radius_x = rx;
    radius_y = ry;
}

/**
 * ���ñ߿������Ĵ�ϸ
 * @param x ������ϸ
 */
void InteractiveButtonBase::setBorderWidth(int x)
{
    border_width = x;
}

/**
 * ���ò����������Ĭ��Ϊ�٣�
 * ������ setEnabled(bool)�������෴�ģ������Ǹ��Ƿ���
 * @param dis ������
 */
void InteractiveButtonBase::setDisabled(bool dis)
{
    if (dis == !isEnabled()) // ��ͬ��
        return ;

    setEnabled(!dis);

    if (parentWidget()!=nullptr)
    {
        setAttribute(Qt::WA_TransparentForMouseEvents, dis); // �����͸
    }

    if (model == PixmapMask || model == PixmapText)
    {
        pixmap = getMaskPixmap(pixmap, dis?getOpacityColor(icon_color):icon_color);
    }

    update(); // �޸�͸����
}

/**
 * ����ǰ���������ߵ� paddings
 * @param l ��߿հ�
 * @param r �ұ߿հ�
 * @param t ���߿հ�
 * @param b �ױ߿հ�
 */
void InteractiveButtonBase::setPaddings(int l, int r, int t, int b)
{
    icon_paddings.left = l;
    icon_paddings.right = r;
    icon_paddings.top = t;
    icon_paddings.bottom = b;
    setFixedForeSize();
}

/**
 * ͳһ���÷���� paddings
 * @param h ����
 * @param v ����
 */
void InteractiveButtonBase::setPaddings(int h, int v)
{
    icon_paddings.left = icon_paddings.right = (h+1) / 2;
    icon_paddings.top = icon_paddings.bottom = (v+1) / 2;
    setFixedForeSize();
}

/**
 * ͳһ����ǰ���������ߵ� paddings
 * @param x һ����С���ı�����
 */
void InteractiveButtonBase::setPaddings(int x)
{
    icon_paddings.left = x;
    icon_paddings.right = x;
    icon_paddings.top = x;
    icon_paddings.bottom = x;
    setFixedForeSize();
}

void InteractiveButtonBase::setIconPaddingProper(double x)
{
    icon_padding_proper = x;
    int short_side = min(geometry().width(), geometry().height()); // �̱�
    // �ǹ̶���������ߴ��С����֮������ padding ��Ҫ��
    int padding = short_side*icon_padding_proper; //static_cast<int>(short_side * (1 - GOLDEN_RATIO) / 2);
    icon_paddings.left = icon_paddings.top = icon_paddings.right = icon_paddings.bottom = padding;
    update();
}

/**
 * ���������Сʱ�Ƿ�ͬ���޸İ�ť����С�ߴ磨���ⰴť��ʾ��ȫ��
 * @param d ����
 */
void InteractiveButtonBase::setTextDynamicSize(bool d)
{
    text_dynamic_size = d;
}

/**
 * �� setFixedForePos(bool f)
 */
void InteractiveButtonBase::setFixedTextPos(bool f)
{
    fixed_fore_pos = f;
}

/**
 * ����ǰ���Ƿ�̶��������ƶ�
 * ��ȥ��������뿿��������Ч����ͳһͼ�������С����
 * ֻ������������/�����������Ϊ����Ч����Ĭ�ϣ�
 * ��Ӱ���κ���������
 * @param f [description]
 */
void InteractiveButtonBase::setFixedForePos(bool f)
{
    fixed_fore_pos = f;
}

/**
 * �̶���ťΪ�ʵ��ߴ磬���ҹ̶���������
 * ǰ��ӦΪ����/ͼ���Ӧ�ߴ����С�ߴ�
 * @param f     �Ƿ�̶�ǰ��
 * @param addin ���׵����ش�С
 */
void InteractiveButtonBase::setFixedForeSize(bool f, int addin)
{
    fixed_fore_size = f;

    if (!f) return ;
    if (model == PaintModel::Text)
    {
        QFont font = this->font();
        if (font_size > 0)
            font.setPointSize(font_size);
        QFontMetrics fm(font);
        setMinimumSize(
            fm.horizontalAdvance(text)+icon_paddings.left+icon_paddings.right+addin,
            fm.lineSpacing()+icon_paddings.top+icon_paddings.bottom+addin
        );
    }
    else if (model == PaintModel::Icon || model == PaintModel::PixmapMask)
    {
        int size = height();
        setMinimumSize(size+addin, size+addin);
    }
}

void InteractiveButtonBase::setSquareSize()
{
    setFixedWidth(height());
    setMinimumWidth(height());
    setMaximumWidth(height());
}

/**
 * ������굥���ɿ����Ƿ����ƿ�
 * ����˵����������ֺ����������Ȼ���ڰ�ť���棬����������ʾ hover ����
 * @param l ����
 */
void InteractiveButtonBase::setLeaveAfterClick(bool l)
{
    leave_after_clicked = l;
}

/**
 * ��Ӧ˫���¼�
 * ע�⣺���ȴ��������¼����ٴ���˫���¼�(��ʵ����������)
 * ������ QListWidget �ȵط�ʹ�ã�
 * @param e ����
 */
void InteractiveButtonBase::setDoubleClicked(bool e)
{
    double_clicked = e;

    if (double_timer == nullptr)
    {
        double_timer = new QTimer(this);
        double_timer->setInterval(DOUBLE_PRESS_INTERVAL);
        connect(double_timer, &QTimer::timeout, [=]{
            double_timer->stop();
            emit clicked(); // �ֶ����������¼�
        });
    }
}

/**
 * �Ƿ������ֶ���
 * �����밴ť����ǰ��ͼ��Ӷ��淽�����ų���
 * @param enable ����
 */
void InteractiveButtonBase::setShowAni(bool enable)
{
    show_animation = enable;

    if (!show_animation) // �ر�����ǰ��
    {
        show_foreground = true;
    }
    else if (show_animation) // ��������ǰ��
    {
        if (!hovering && !pressing) // Ӧ��������״̬
        {
            show_ani_appearing = show_ani_disappearing = show_foreground = false;
            show_ani_progress = 0;
        }
        else // Ӧ������ʾ״̬
        {
            show_foreground = true;
            show_ani_appearing = show_ani_disappearing = false;
            show_ani_progress = 100;
        }
    }
}

/**
 * ��ťǰ�����ֶ���
 * �����ĵ���ֵ����Ŷ���
 */
void InteractiveButtonBase::showForeground()
{
    if (!show_animation) return ;
    waters.clear();
    if (!anchor_timer->isActive())
        anchor_timer->start();
    if (show_ani_disappearing)
        show_ani_disappearing = false;
    show_ani_appearing = true;
    show_timestamp = getTimestamp();
    show_foreground = true;
    show_ani_point = QPoint(0,0);
}

/**
 * ��ťǰ�����ֶ���2
 * ָ�����򣨵ѿ������꣩���ӷ����������ĵ�
 * @param point �ʼ���ֵķ��򣨴�С��Ӱ�죬ֻ�� x��y ��������
 */
void InteractiveButtonBase::showForeground2(QPoint point)
{
    showForeground();
    if (point == QPoint(0,0))
        point = mapFromGlobal(QCursor::pos()) - QPoint(size().width()/2, size().height()/2); // ����ڰ�ť����
    show_ani_point = point;

    if (unified_geometry) // ͳһ���ֶ���
        updateUnifiedGeometry();
}

/**
 * ����ǰ��
 * Ϊ��һ�εĳ��ֶ�����׼��
 */
void InteractiveButtonBase::hideForeground()
{
    if (!show_animation) return ;
    if (!anchor_timer->isActive())
        anchor_timer->start();
    if (show_ani_appearing)
        show_ani_appearing = false;
    show_ani_disappearing = true;
    hide_timestamp = getTimestamp();
}

/**
 * �ӳٳ���ǰ��
 * �����ڶ����ť�������ֵ�һ��Ч��
 * @param time  �ӳ�ʱ�������룩
 * @param point ���ַ���
 */
void InteractiveButtonBase::delayShowed(int time, QPoint point)
{
    setShowAni(true);
    QTimer::singleShot(time, [=]{
        showForeground2(point);
        connect(this, &InteractiveButtonBase::showAniFinished, [=]{
            setShowAni(false);
            disconnect(this, SIGNAL(showAniFinished()), nullptr, nullptr);
        });
    });
}

/**
 * ���ò˵�
 * ������˵��޷������� release ������
 * @param menu �˵�����
 */
void InteractiveButtonBase::setMenu(QMenu *menu)
{
    // Ĭ�������˲���ȡ�����¼���������������˲˵��Ļ����Ͳ�����Release�¼���ˮ���ƶ�����һֱƮ��
    // �� focusOut �¼��У�ģ���� release �¼���
    this->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    QPushButton::setMenu(menu);
}

/**
 * ����״̬
 * һ��������Ϊ����Ч��������
 * @param s ״̬
 */
void InteractiveButtonBase::setState(bool s)
{
    _state = s;
    update();
}

/**
 * ��ȡ״̬
 * @return ״̬
 */
bool InteractiveButtonBase::getState()
{
    return _state;
}

/**
 * ģ�ⰴ�¿��ص�Ч�������ı�״̬
 * �����ʹ��״̬������ֵ������
 */
void InteractiveButtonBase::simulateStatePress(bool s)
{
    if (getState() == s)
        return ;

    if (inArea(mapFromGlobal(QCursor::pos()))) // �����ǰ��ť������Ҫ��ģ����
        return ;

    mousePressEvent(new QMouseEvent(QMouseEvent::Type::None, QPoint(size().width()/2,size().height()/2), Qt::LeftButton, Qt::NoButton, Qt::NoModifier));

    mouseReleaseEvent(new QMouseEvent(QMouseEvent::Type::None, QPoint(size().width()/2,size().height()/2), Qt::LeftButton, Qt::NoButton, Qt::NoModifier));

    // if (!inArea(mapFromGlobal(QCursor::pos()))) // ���ģ��release ���� // �ض�����
        hovering = false;
}

/**
 * ��������¼������� hover ʱ���
 */
void InteractiveButtonBase::enterEvent(QEvent *event)
{
    if (!anchor_timer->isActive())
    {
        anchor_timer->start();
    }
    hovering = true;
    hover_timestamp = getTimestamp();
    leave_timestamp = 0;
    if (mouse_pos == QPoint(-1,-1))
        mouse_pos = mapFromGlobal(QCursor::pos());

    return QPushButton::enterEvent(event);
}

/**
 * ����ƿ��¼������� leave ʱ���
 */
void InteractiveButtonBase::leaveEvent(QEvent *event)
{
    hovering = false;
    if (!pressing)
        mouse_pos = QPoint(geometry().width()/2, geometry().height()/2);

    return QPushButton::leaveEvent(event);
}

/**
 * ��갴���¼������� press ʱ���
 * ���ˮ���ƶ��� waters ����
 */
void InteractiveButtonBase::mousePressEvent(QMouseEvent *event)
{
    mouse_pos = event->pos();

    if (event->button() == Qt::LeftButton)
    {
        if (!hovering)
            InteractiveButtonBase::enterEvent(new QEvent(QEvent::Type::None));

        pressing = true;
        press_pos = mouse_pos;
        // �ж�˫���¼�
        if (double_clicked)
        {
            qint64 last_press_timestamp = press_timestamp;
            press_timestamp = getTimestamp();
            if (release_timestamp+DOUBLE_PRESS_INTERVAL>=press_timestamp
                    && last_press_timestamp+SINGLE_PRESS_INTERVAL>release_timestamp
                    && release_pos==press_pos) // ��˫��(�ж����ε����ļ��)
            {
                double_timer->stop();
                emit doubleClicked();
                double_prevent = true; // ��ֹ���ε�releaseʶ��Ϊ˫��
                return ;
            }
            else
            {
                double_prevent = false; // �����ж���� bug
            }
        }
        else
        {
            press_timestamp = getTimestamp();
        }

        if (water_animation)
        {
            waters << Water(press_pos, press_timestamp);
        }
        else // ͸������
        {
            if (press_progress < press_start)
                press_progress = press_start; // ֱ������Ϊ����Ч����ʼֵ�����ⰴ�·�Ӧ����
        }
    }

    return QPushButton::mousePressEvent(event);
}

/**
 * ����ɿ��¼������� release ʱ���
 * ��Ӷ������� jitters ����
 */
void InteractiveButtonBase::mouseReleaseEvent(QMouseEvent* event)
{
    if (pressing && event->button() == Qt::LeftButton)
    {
        if (!inArea(event->pos()))
        {
            hovering = false;
        }
        pressing = false;
        release_pos = event->pos();
        release_timestamp = getTimestamp();

        // ��Ӷ���Ч��
        if (jitter_animation)
        {
            setJitter();
        }

        if (water_animation && waters.size())
        {
            waters.last().release_timestamp = release_timestamp;
        }

        if (double_clicked)
        {
            if (double_prevent) // ˫���ĵ���release�������뵥������
            {
                double_prevent = false;
                return ;
            }

            // Ӧ�ò���˫���Ĳ���
            if (release_pos != press_pos || release_timestamp - press_timestamp >= SINGLE_PRESS_INTERVAL)
            {

            }
            else // ������˫����׼��
            {
                double_timer->start();
                return ; // ��ֹ�����¼�
            }
        }
    }

    return QPushButton::mouseReleaseEvent(event);
}

/**
 * ����ƶ��¼�
 */
void InteractiveButtonBase::mouseMoveEvent(QMouseEvent *event)
{
    if (hovering == false) // ʧȥ�����ֻ�����
    {
        enterEvent(nullptr);
    }
    mouse_pos = mapFromGlobal(QCursor::pos());

    return QPushButton::mouseMoveEvent(event);
}

/**
 * �ߴ��С�ı��¼�
 * ͬ�������ͳߴ��йص���������
 */
void InteractiveButtonBase::resizeEvent(QResizeEvent *event)
{
    if (!pressing && !hovering)
    {
        mouse_pos = QPoint(geometry().width()/2, geometry().height()/2);
        anchor_pos = mouse_pos;
    }
    water_radius = static_cast<int>(max(geometry().width(), geometry().height()) * 1.42); // ����
    // �ǹ̶���������ߴ��С����֮������ padding ��Ҫ��
    if (model == PaintModel::Icon || model == PaintModel::PixmapMask)
    {
        int short_side = min(geometry().width(), geometry().height()); // �̱�
        int padding = short_side*icon_padding_proper; //static_cast<int>(short_side * (1 - GOLDEN_RATIO) / 2);
        icon_paddings.left = icon_paddings.top = icon_paddings.right = icon_paddings.bottom = padding;
    }
    _l = _t = 0; _w = size().width(); _h = size().height();

    return QPushButton::resizeEvent(event);
}

/**
 * ��ý����¼�
 * �Ѿ�ȡ����ť��ȡ���㣬focusIn��focusOut�¼������ᴥ��
 */
void InteractiveButtonBase::focusInEvent(QFocusEvent *event)
{
    if (!hovering && inArea(mapFromGlobal(QCursor::pos())))
        InteractiveButtonBase::enterEvent(new QEvent(QEvent::Type::None));

    return QPushButton::focusInEvent(event);
}

/**
 * ʧȥ�����¼�
 * ���ݰ�סʱͻȻʧȥ���㣨���絯���˵������������������ˣ�
 */
void InteractiveButtonBase::focusOutEvent(QFocusEvent *event)
{
    if (hovering)
    {
        hovering = false;
    }
    if (pressing) // ���һֱ��ס��������click�¼����ƶ��˽���
    {
        pressing = false;
        release_pos = mapFromGlobal(QCursor::pos());
        release_timestamp = getTimestamp();

        if (water_animation && waters.size())
        {
            waters.last().release_timestamp = release_timestamp;
        }
    }

    return QPushButton::focusOutEvent(event);
}

/**
 * �ػ��¼�
 * �����������ݣ�������������ǰ�����Ǳ�
 */
void InteractiveButtonBase::paintEvent(QPaintEvent* event)
{
    if (parent_enabled) // ���Ƹ��ࣨ�Ա�ʹ�ø����QSS�͸������ԣ�
        QPushButton::paintEvent(event);
    if (!self_enabled) // �������Լ�
        return ;
    QPainter painter(this);

    // ==== ���Ʊ��� ====
    QPainterPath path_back = getBgPainterPath();
    painter.setRenderHint(QPainter::Antialiasing,true);

    if (normal_bg.alpha() != 0) // Ĭ�ϱ���
    {
        painter.fillPath(path_back, isEnabled()?normal_bg:getOpacityColor(normal_bg));
    }

    if (border_bg.alpha() != 0 && border_width > 0)
    {
        painter.save();
        QPen pen;
        pen.setColor(border_bg);
        pen.setWidth(border_width);
        painter.setPen(pen);
        painter.drawPath(path_back);
        painter.restore();
    }

    if (hover_progress) // ��������
    {
         painter.fillPath(path_back, getOpacityColor(hover_bg, hover_progress / 100.0));
    }

    if (press_progress && !water_animation) // ���½��䵭����ʧ
    {
        painter.fillPath(path_back, getOpacityColor(press_bg, press_progress/100.0));
    }
    else if (water_animation && waters.size()) // ˮ���ƣ���������һ��ˮ����
    {
        paintWaterRipple(painter);
    }

    // ==== ����ǰ�� ====
    if (fore_enabled/*��԰�ť����*/ && show_foreground/*��Զ�������*/)
    {
        painter.setPen(isEnabled()?icon_color:getOpacityColor(icon_color));

        // ���ƶ������ݣ����ܱ�ǰ�����ǣ�
        if (paint_addin.enable)
        {
            int l = icon_paddings.left, t = icon_paddings.top, r = size().width()-icon_paddings.right, b = size().height()-icon_paddings.bottom;
            int small_edge = min(size().height(), size().width());
            int pw = paint_addin.size.width() ? paint_addin.size.width() : small_edge-icon_paddings.left-icon_paddings.right;
            int ph = paint_addin.size.height() ? paint_addin.size.height() : small_edge-icon_paddings.top-icon_paddings.bottom;
            if (paint_addin.align & Qt::AlignLeft)
                r = l + pw;
            else if (paint_addin.align & Qt::AlignRight)
                l = r - pw;
            else if (paint_addin.align & Qt::AlignHCenter)
            {
                l = size().width()/2-pw/2;
                r = l+pw;
            }
            if (paint_addin.align & Qt::AlignTop)
                b = t + ph;
            else if (paint_addin.align & Qt::AlignBottom)
                t = b - ph;
            else if (paint_addin.align & Qt::AlignVCenter)
            {
                t = size().height()/2-ph/2;
                b = t+ph;
            }
            painter.drawPixmap(QRect(l,t,r-l,b-t), paint_addin.pixmap);
        }

        QRect rect(icon_paddings.left+(fixed_fore_pos?0:offset_pos.x()), icon_paddings.top+(fixed_fore_pos?0:offset_pos.y()), // ԭ����λ�ã����������������Ч��
                   (size().width()-icon_paddings.left-icon_paddings.right),
                   size().height()-icon_paddings.top-icon_paddings.bottom);

        // �������ֶ���
        if ((show_ani_appearing || show_ani_disappearing) && show_ani_point != QPoint( 0, 0 ) && ! fixed_fore_pos)
        {
            //int w = size().width(), h = size().height();
            int pro = getSpringBackProgress(show_ani_progress, 50);

            // show_ani_point ��������ĵ㣬��ô��ʼ����Ӧ�����෴��
            int x = show_ani_point.x(), y = show_ani_point.y();
            int gen = quick_sqrt(x*x + y*y);
            x = water_radius * x / gen; // ������ʼ���ĵ������ ����
            y = water_radius * y / gen; // ������ʼ���ĵ������� ����

            rect = QRect(
                rect.left() - x * (100-pro) / 100 + rect.width() * (100-pro) / 100,
                rect.top() - y * (100-pro) / 100 + rect.height() * (100-pro) / 100,
                rect.width() * pro / 100,
                rect.height() * pro / 100
                );

        }
        else if (align == Qt::AlignCenter && model != PaintModel::Text && !fixed_fore_size) // Ĭ�ϵ����Ŷ���
        {
            int delta_x = 0, delta_y = 0;
            if (click_ani_progress != 0) // ͼ������
            {
                delta_x = rect.width() * click_ani_progress / 400;
                delta_y = rect.height() * click_ani_progress / 400;
            }
            else if (show_ani_appearing)
            {
                /*int pro; // ����������ת��Ϊ�ص���������
                if (show_ani_progress <= 50)
                    pro = show_ani_progress * 2;
                else if (show_ani_progress <= 75)
                    pro = (show_ani_progress-50)/2 + 100;
                else
                    pro = 100 + (100-show_ani_progress)/2;

                delta_x = rect.width() * (100-pro) / 100;
                delta_y = rect.height() * (100-pro) / 100;*/

                double pro = getNolinearProg(show_ani_progress, SpringBack50);
                delta_x = static_cast<int>(rect.width() * (1-pro));
                delta_y = static_cast<int>(rect.height() * (1-pro));
            }
            else if (show_ani_disappearing)
            {
                double pro = 1 - getNolinearProg(show_ani_progress, SlowFaster);
                delta_x = rect.width() * pro; // (100-show_ani_progress) / 100;
                delta_y = rect.height() * pro; // (100-show_ani_progress) / 100;
            }
            if (delta_x || delta_y)
                rect = QRect(rect.left()+delta_x, rect.top()+delta_y,
                            rect.width()-delta_x*2, rect.height()-delta_y*2);
        }

        /*if (this->isEnabled())
        {
            QColor color = icon_color;
            color.setAlpha(color.alpha() / 2);
            painter.setPen(color);
        }*/

        if (model == None)
        {
            // �����Լ��Ļ�������
        }
        else if (model == Text)
        {
            // �������ֽ̳̣� https://blog.csdn.net/temetnosce/article/details/78068464
            painter.setPen(isEnabled()?text_color:getOpacityColor(text_color));
            /*if (show_ani_appearing || show_ani_disappearing)
            {
                int pro = getSpringBackProgress(show_ani_progress, 50);
                QFont font = painter.font();
                int ps = font.pointSize();
                ps = ps * show_ani_progress / 100;
                font.setPointSize(ps);
                painter.setFont(font);
            }*/
            if (font_size > 0)
            {
                QFont font = painter.font();
                font.setPointSize(font_size);
                painter.setFont(font);
            }
            painter.drawText(rect, static_cast<int>(align), text);
        }
        else if (model == Icon) // ����ͼ��
        {
            icon.paint(&painter, rect, align);
        }
        else if (model == PixmapMask)
        {
            painter.setRenderHint(QPainter::SmoothPixmapTransform, true); // �����ñ�Ե������ƽ��һЩ
            painter.drawPixmap(rect, pixmap);
        }
        else if (model == PixmapText)
        {

        }
    }

    // ==== �������λ�� ====
//    painter.drawEllipse(QRect(anchor_pos.x()-5, anchor_pos.y()-5, 10, 10)); // �ƶ�ê��
//    painter.drawEllipse(QRect(effect_pos.x()-2, effect_pos.y()-2, 4, 4)); // Ӱ��λ��ê��

    //    return QPushButton::paintEvent(event); // �����Ƹ��౳����
}

/**
 * �ж������Ƿ��ڰ�ť������
 * ����ʧȥ�˽��㣬����������Ҫ hover Ч�����ǲ˵��͵������߽��㣩
 * Ϊ�������ΰ�ť�����ж��ṩ֧��
 * @param  point ��ǰ���
 * @return       �Ƿ���������
 */
bool InteractiveButtonBase::inArea(QPoint point)
{
    return !(point.x() < 0 || point.y() < 0 || point.x() > size().width() || point.y() > size().height());
}

/**
 * ��ȡ��ť�����Ļ�������
 * Ϊ�������ΰ�ť�ṩ֧��
 * @return [description]
 */
QPainterPath InteractiveButtonBase::getBgPainterPath()
{
    QPainterPath path;
    if (radius_x || radius_y)
        path.addRoundedRect(QRect(0,0,size().width(),size().height()), radius_x, radius_y);
    else
        path.addRect(QRect(0,0,size().width(),size().height()));
    return path;
}

/**
 * ��ȡˮ���ƻ�������Բ�Σ�������������
 * Բ��ˮ�� & ��ť����
 * @param  water һ��ˮ���ƶ�������
 * @return       ����·��
 */
QPainterPath InteractiveButtonBase::getWaterPainterPath(InteractiveButtonBase::Water water)
{
    double prog = getNolinearProg(water.progress, FastSlower);
    int ra = water_radius*prog;
    QRect circle(water.point.x() - ra,
                water.point.y() - ra,
                ra*2,
                ra*2);
    /*QRect circle(water.point.x() - water_radius*water.progress/100,
                water.point.y() - water_radius*water.progress/100,
                water_radius*water.progress/50,
                water_radius*water.progress/50);*/
    QPainterPath path;
    path.addEllipse(circle);
    if (radius_x || radius_y)
        return path & getBgPainterPath();
    return path;
}

/**
 * ��ȡͳһ�ĳߴ��С���ѷ�����
 * ����Բ�ΰ�ť���ֶ������뾶ʹ��ˮ���ƣ��Խ��ߣ�
 * ��ֱ��ʹ�� protected ����
 * @return ǰ����������
 */
QRect InteractiveButtonBase::getUnifiedGeometry()
{
    // ����������ת��Ϊ�ص���������
    int pro = show_ani_appearing ? getSpringBackProgress(show_ani_progress,50) : show_ani_progress;
    int ul = 0, ut = 0, uw = size().width(), uh = size().height();

    // show_ani_point ��������ĵ㣬��ô��ʼ����Ӧ�����෴��
    int x = show_ani_point.x(), y = show_ani_point.y();
    int gen = quick_sqrt(x*x + y*y);
    x = - water_radius * x / gen; // ������ʼ���ĵ������ ����
    y = - water_radius * y / gen; // ������ʼ���ĵ������� ����

    ul = ul + x * (100-pro) / 100 + uw * (100-pro) / 200;
    ut = ut + y * (100-pro) / 100 + uh * (100-pro) / 200;
    uw = uw * pro / 100;
    uh = uh * pro / 100;

    return QRect(ul, ut, uw, uh);
}

/**
 * ����ͳһ��������
 * �ڲ��� _l, _t, _w, _h ��ֱ��ʹ��
 */
void InteractiveButtonBase::updateUnifiedGeometry()
{
    _l = 0; _t = 0; _w = geometry().width(); _h = geometry().height();
    if ((show_ani_appearing || show_ani_disappearing) && show_ani_point != QPoint( 0, 0 ))
    {
        int pro; // ����������ת��Ϊ�ص���������
        pro = show_ani_appearing ? getSpringBackProgress(show_ani_progress,50) : show_ani_progress;

        // show_ani_point ��������ĵ㣬��ô��ʼ����Ӧ�����෴��
        int x = show_ani_point.x(), y = show_ani_point.y();
        int gen = quick_sqrt(x*x + y*y);
        x = - water_radius * x / gen; // ������ʼ���ĵ������ ����
        y = - water_radius * y / gen; // ������ʼ���ĵ������� ����

        _l = _l + x * (100-pro) / 100 + _w * (100-pro) / 200;
        _t = _t + y * (100-pro) / 100 + _h * (100-pro) / 200;
        _w = _w * pro / 100;
        _h = _h * pro / 100;
    }
}

/**
 * ����һ��ˮ���ƶ���
 * @param painter ���ƶ��󣨼�painter(this)����
 */
void InteractiveButtonBase::paintWaterRipple(QPainter& painter)
{
    QColor water_finished_color(press_bg);

    for (int i = 0; i < waters.size(); i++)
    {
        Water water = waters.at(i);
        if (water.finished) // ������ʧ
        {
            water_finished_color.setAlpha(press_bg.alpha() * water.progress / 100);
            QPainterPath path_back = getBgPainterPath();
//                painter.setPen(water_finished_color);
            painter.fillPath(path_back, QBrush(water_finished_color));
        }
        else // Բ�γ���
        {
            QPainterPath path = getWaterPainterPath(water);
            painter.fillPath(path, QBrush(press_bg));
        }
    }
}

/**
 * ����ɿ���ʱ�򣬼������ж���Ч����·�����¼�
 * ��ÿ���ػ�����ʱ�����α������е�·��
 */
void InteractiveButtonBase::setJitter()
{
    jitters.clear();
    QPoint center_pos = geometry().center()-geometry().topLeft();
    int full_manh = (anchor_pos-center_pos).manhattanLength(); // ����
    if (full_manh > (geometry().topLeft() - geometry().bottomRight()).manhattanLength()) // ���볬�����Բ�뾶����������
    {
        QPoint jitter_pos(effect_pos);
        full_manh = (jitter_pos-center_pos).manhattanLength();
        int manh = full_manh;
        int duration = jitter_duration;
        qint64 timestamp = release_timestamp;
        while (manh > elastic_coefficient)
        {
            jitters << Jitter(jitter_pos, timestamp);
            jitter_pos = center_pos - (jitter_pos - center_pos) / elastic_coefficient;
            duration = jitter_duration * manh / full_manh;
            timestamp += duration;
            manh = static_cast<int>(manh / elastic_coefficient);
        }
        jitters << Jitter(center_pos, timestamp);
        anchor_pos = mouse_pos = center_pos;
    }
}

/**
 * �ٶȼ���Ŀ����㷨��Ч��δ֪��ԭ��δ֪
 * @param  X ������������
 * @return   ƽ����
 */
int InteractiveButtonBase::quick_sqrt(long X) const
{
#if !defined(Q_OS_WIN)
    return qSqrt(X);
#endif
    bool fu = false;
    if (X < 0)
    {
        fu = true;
        X = -X;
    }
    unsigned long M = static_cast<unsigned long>(X);
    unsigned int N, i;
    unsigned long tmp, ttp; // �����ѭ������
    if (M == 0) // �����������������ҲΪ0
        return 0;
    N = 0;
    tmp = (M >> 30); // ��ȡ���λ��B[m-1]
    M <<= 2;
    if (tmp > 1) // ���λΪ1
    {
        N ++; // �����ǰλΪ1������ΪĬ�ϵ�0
        tmp -= N;
    }
    for (i = 15; i > 0; i--) // ��ʣ���15λ
    {
        N <<= 1; // ����һλ
        tmp <<= 2;
        tmp += (M >> 30); // ����
        ttp = N;
        ttp = (ttp << 1) + 1;
        M <<= 2;
        if (tmp >= ttp) // �������
        {
            tmp -= ttp;
            N ++;
        }
    }
    return (fu ? -1 : 1) * static_cast<int>(N); // ��֪��Ϊʲô��������Ľ���Ƿ�������
}

/**
 * ���ֵ
 */
int InteractiveButtonBase::max(int a, int b) const { return a > b ? a : b; }

/**
 * ��Сֵ
 */
int InteractiveButtonBase::min(int a, int b) const { return a < b ? a : b; }

/**
 * ��ȡ����ʱ�����13λ����ȷ������
 * @return ʱ���
 */
qint64 InteractiveButtonBase::getTimestamp() const
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

/**
 * �Ƿ�Ϊ��ɫ��ɫ
 * @param  color ��ɫ
 * @return       �Ƿ�Ϊ��ɫ
 */
bool InteractiveButtonBase::isLightColor(QColor color)
{
    return color.red()*0.299 + color.green()*0.578 + color.blue()*0.114 >= 192;
}

/**
 * ��ȡ�����Զ�����ĳһʱ������Ķ�������
 * �������ڵ���ͷЧ���Ķ���
 * @param  x   ʵ���������100%�Ķ�������
 * @param  max ǰ�벿�ֶ�����������
 * @return     Ӧ����ʾ�Ķ�������
 */
int InteractiveButtonBase::getSpringBackProgress(int x, int max)
{
    if (x <= max)
        return x * 100 / max;
    if (x <= max + (100-max)/2)
        return (x-max)/2+100;
    return 100 + (100-x)/2;
}

/**
 * ��ȡ͸������ɫ
 * @param  color ��ɫ
 * @param  level ����
 * @return       ͸����ɫ
 */
QColor InteractiveButtonBase::getOpacityColor(QColor color, double level)
{
    color.setAlpha(static_cast<int>(color.alpha() * level));
    return color;
}

/**
 * ��ȡ��Ӧ��ɫ��ͼ�� pixmap
 * @param  p ͼ��
 * @param  c ��ɫ
 * @return   ��Ӧ��ɫ��ͼ��
 */
QPixmap InteractiveButtonBase::getMaskPixmap(QPixmap p, QColor c)
{
    QBitmap mask = p.mask();
    p.fill(c);
    p.setMask(mask);
    return p;
}

double InteractiveButtonBase::getNolinearProg(int p, InteractiveButtonBase::NolinearType type)
{
    if (p <= 0)
        return 0.0;
    if (p >= 100)
        return 1.0;

    switch (type)
    {
    case Linear:
        return p / 100.0;
    case SlowFaster:
        return p * p / 10000.0;
    case FastSlower :
        return quick_sqrt(p*100) / 100.0;
    case SlowFastSlower:
        if (p <= 50)
            return p * p / 50.0;
        else
            return 0.5 + quick_sqrt(50*(p-50))/100.0;
    case SpringBack20:
    case SpringBack50:
        if (p <= 50)
            return p / 50.0;
        else if (p < 75)
            return 1.0 + (p-50) / 200.0;
        else
            return 1.0 + (100-p) / 200.0;
    }
}

/**
 * ê���ɵ����λ�õĶ�ʱʱ��
 * ͬ���������к�ʱ�����֡���йصĶ���������
 */
void InteractiveButtonBase::anchorTimeOut()
{
    qint64 timestamp = getTimestamp();
    // ==== ����ɫ ====
    /*if (hovering) // �ڿ��ڣ�����
    {
        if (hover_progress < 100) // ���жϣ��ټ��㣬�ɽ�Լ������Դ
            hover_progress = min((timestamp - hover_timestamp) * 100 / press_bg_duration, 100);
    }
    else // �ڿ��⣺��ǳ
    {
        if (hover_progress > 0)
            hover_progress = max((timestamp - leave_timestamp) * 100 / press_bg_duration, 0);
    }

    if (pressing)
    {
        if (press_progress < 100)
            press_progress = min(press_start + (timestamp - press_timestamp) * 100 / press_bg_duration, 100);
    }
    else
    {
        if (press_progress > 0) // ������µ�Ч�����ڣ���ǳ
            press_progress = max((timestamp - release_timestamp) * 100 / press_bg_duration, 0);
    }*/

    if (pressing) // ��갴��
    {
        if (press_progress < 100) // ͸�����䣬��û�����
        {
            press_progress += press_speed;
            if (press_progress > 100)
                press_progress = 100;
        }
        if (hovering && hover_progress < 100)
        {
            hover_progress += hover_speed;
            if (hover_progress > 100)
                hover_progress = 100;
        }
    }
    else // �������
    {
        if (press_progress>0) // ������µ�Ч�����ڣ���ǳ
        {
            press_progress -= press_speed;
            if (press_progress < 0)
                press_progress = 0;
        }

        if (hovering) // �ڿ��ڣ�����
        {
            if (hover_progress < 100)
            {
                hover_progress += hover_speed;
                if (hover_progress > 100)
                    hover_progress = 100;
            }
        }
        else // �ڿ��⣺��ǳ
        {
            if (hover_progress > 0)
            {
                hover_progress -= hover_speed;
                if (hover_progress < 0)
                    hover_progress = 0;
            }
        }
    }

    // ==== ���±���ˮ���ƶ��� ====
    if (water_animation)
    {
        for (int i = 0; i < waters.size(); i++)
        {
            Water& water = waters[i];
            if (water.finished)
            {
                water.progress = static_cast<int>(100 - 100 * (timestamp-water.finish_timestamp) / water_finish_duration);
                if (water.progress <= 0)
                    waters.removeAt(i--);
            }
            else // ���ڳ���״̬
            {
                if (water.progress >= 100)
                {
                    water.progress = 100;
                    if (water.release_timestamp) // ����Ѿ��ɿ���
                    {
                        water.finished = true; // ׼������
                        water.finish_timestamp = timestamp;
                    }
                }
                else // �����е�
                {
                    if (water.release_timestamp) // ����Ѿ��ɿ���
                    {
                        water.progress = static_cast<int>(100 * (water.release_timestamp - water.press_timestamp) / water_press_duration
                                + 100 * (timestamp - water.release_timestamp) / water_release_duration);
                    }
                    else // ���һֱ����
                    {
                        water.progress = static_cast<int>(100 * (timestamp - water.press_timestamp) / water_press_duration);
                    }
                    if (water.progress > 100)
                        water.progress = 100;
                }
            }
        }
    }

    // ==== ���ֶ��� ====
    if (show_animation)
    {
        if (show_ani_appearing) // ����
        {
            qint64 delta = getTimestamp() - show_timestamp;
            if (show_ani_progress >= 100) // ���ֽ���
            {
                show_ani_appearing = false;
                emit showAniFinished();
            }
            else
            {
                show_ani_progress = static_cast<int>(100 * delta / show_duration);
                if (show_ani_progress > 100)
                    show_ani_progress = 100;
            }
        }
        if (show_ani_disappearing) // ��ʧ
        {
            qint64 delta = getTimestamp() - hide_timestamp;
            if (show_ani_progress <= 0) // ��ʧ����
            {
                show_ani_disappearing = false;
                show_foreground = false;
                show_ani_point = QPoint(0,0);
                emit hideAniFinished();
            }
            else
            {
                show_ani_progress = static_cast<int>(100 - 100 * delta / show_duration);
                if (show_ani_progress < 0)
                    show_ani_progress = 0;
            }
        }
    }

    // ==== ���¶��� ====
    if (click_ani_disappearing) // �������Ч����ʧ
    {
        qint64 delta = getTimestamp()-release_timestamp-click_ani_duration;
        if (delta <= 0) click_ani_progress = 100;
        else click_ani_progress = static_cast<int>(100 - delta*100 / click_ani_duration);
        if (click_ani_progress < 0)
        {
            click_ani_progress = 0;
            click_ani_disappearing = false;
            emit pressAppearAniFinished();
        }
    }
    if (click_ani_appearing) // �������Ч��
    {
        qint64 delta = getTimestamp()-release_timestamp;
        if (delta <= 0) click_ani_progress = 0;
        else click_ani_progress = static_cast<int>(delta * 100 / click_ani_duration);
        if (click_ani_progress > 100)
        {
            click_ani_progress = 100; // ����100��״̬���´ε��ʱ�ص�0
            click_ani_appearing = false;
            click_ani_disappearing = true;
            emit pressDisappearAniFinished();
        }
    }

    // ==== ê���ƶ� ====
    if (jitters.size() > 0) // �ɿ�ʱ�Ķ���Ч��
    {
        // ��ǰӦ���Ǵ������һ����
        Jitter cur = jitters.first();
        Jitter aim = jitters.at(1);
        int del = static_cast<int>(getTimestamp()-cur.timestamp);
        int dur = static_cast<int>(aim.timestamp - cur.timestamp);
        effect_pos = cur.point + (aim.point-cur.point)*del/dur;
        offset_pos = effect_pos- (geometry().center() - geometry().topLeft());

        if (del >= dur)
            jitters.removeFirst();

        // ��������
        if (jitters.size() == 1)
        {
            jitters.clear();
            emit jitterAniFinished();
        }
    }
    else if (anchor_pos != mouse_pos) // �ƶ�Ч��
    {
        int delta_x = anchor_pos.x() - mouse_pos.x(),
            delta_y = anchor_pos.y() - mouse_pos.y();

        anchor_pos.setX( anchor_pos.x() - quick_sqrt(delta_x) );
        anchor_pos.setY( anchor_pos.y() - quick_sqrt(delta_y) );

        offset_pos.setX(quick_sqrt(static_cast<long>(anchor_pos.x()-(geometry().width()>>1))));
        offset_pos.setY(quick_sqrt(static_cast<long>(anchor_pos.y()-(geometry().height()>>1))));
        effect_pos.setX( (geometry().width() >>1) + offset_pos.x());
        effect_pos.setY( (geometry().height()>>1) + offset_pos.y());
    }
    else if (!pressing && !hovering && !hover_progress && !press_progress
             && !click_ani_appearing && !click_ani_disappearing && !jitters.size() && !waters.size()
             && !show_ani_appearing && !show_ani_disappearing) // û����Ҫ���ص����ͣ����Լ��Դ��
    {
        anchor_timer->stop();
    }

    // ==== ͳһ����ĳ��ֶ��� ====
    if (unified_geometry)
    {
        updateUnifiedGeometry();
    }

    update();
}

/**
 * ��굥���¼�
 * ʵ�ⰴ�º��ڰ�ť�����𣬲����ƶ����پ��붼���� clicked
 */
void InteractiveButtonBase::slotClicked()
{
    click_ani_appearing = true;
    click_ani_disappearing = false;
    click_ani_progress = 0;
    release_offset = offset_pos;

    jitters.clear(); // �������
}

/**
 * ǿ�йر�״̬
 * �Բ۵���ʽ����������
 */
void InteractiveButtonBase::slotCloseState()
{
    setState(false);
}

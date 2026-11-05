#include "ScreenCaptureController.h"
#include "ScreenCaptureOverlay.h"

#include <QByteArray>
#include <QCoreApplication>
#include <QEvent>
#include <QGuiApplication>
#include <QMetaObject>
#include <QPixmap>
#include <QPointer>
#include <QScreen>
#include <QTimer>
#include <QVector>
#include <QWidget>
#include <QWindow>
#include <utility>

class ScreenCaptureController::Private : public QObject
{
public:
    enum class EndReason { Captured, Cancelled, Error, Silent };

    struct ScreenFrame
    {
        QPointer<QScreen> screen;
        QRect geometry;
        QImage image;
        QPointer<ScreenCaptureOverlay> overlay;
    };

    explicit Private(ScreenCaptureController *controller) : QObject(controller), q(controller) {}

    ~Private() override
    {
        finish(EndReason::Silent, false);
    }

    bool isCurrent(quint64 token) const
    {
        return active && !ending && generation == token && session;
    }

    bool start(QWidget *sourceWindow)
    {
        if (active || ending || !sourceWindow)
            return false;
        QWidget *window = sourceWindow->window();
        auto *application = qobject_cast<QGuiApplication *>(QCoreApplication::instance());
        if (!window || !application)
            return false;

        source = window;
        savedGeometry = window->saveGeometry();
        savedState = window->windowState();
        wasVisible = window->isVisible();
        sourceClosed = false;
        active = true;
        const quint64 token = ++generation;
        session = new QObject(this);
        window->installEventFilter(this);
        connections.append(connect(window, &QObject::destroyed, session, [this, token]() {
            if (!isCurrent(token))
                return;
            source.clear();
            finish(EndReason::Cancelled, false);
        }));

        connections.append(connect(application, &QGuiApplication::screenAdded, session,
                                   [this, token](QScreen *) {
            if (isCurrent(token))
                finish(EndReason::Cancelled);
        }));
        connections.append(connect(application, &QGuiApplication::screenRemoved, session,
                                   [this, token](QScreen *) {
            if (isCurrent(token))
                finish(EndReason::Cancelled);
        }));
        const QList<QScreen *> screens = QGuiApplication::screens();
        for (QScreen *screen : screens) {
            if (!screen)
                continue;
            ScreenFrame frame;
            frame.screen = screen;
            frame.geometry = screen->geometry();
            frames.append(frame);
            connections.append(connect(screen, &QScreen::geometryChanged, session,
                                       [this, token](const QRect &) {
                if (isCurrent(token))
                    finish(EndReason::Cancelled);
            }));
            connections.append(connect(screen, &QScreen::logicalDotsPerInchChanged, session,
                                       [this, token](qreal) {
                if (isCurrent(token))
                    finish(EndReason::Cancelled);
            }));
            connections.append(connect(screen, &QScreen::physicalDotsPerInchChanged, session,
                                       [this, token](qreal) {
                if (isCurrent(token))
                    finish(EndReason::Cancelled);
            }));
            connections.append(connect(screen, &QObject::destroyed, session, [this, token]() {
                if (isCurrent(token))
                    finish(EndReason::Cancelled);
            }));
        }

        delayTimer = new QTimer(session);
        delayTimer->setSingleShot(true);
        connections.append(connect(delayTimer, &QTimer::timeout, session, [this, token]() {
            if (!isCurrent(token))
                return;
            if (!source) {
                finish(EndReason::Cancelled, false);
                return;
            }
            grabScreens(token);
        }));

        QPointer<ScreenCaptureController> owner(q);
        window->hide();
        if (!owner || !isCurrent(token))
            return true;
        if (!source) {
            finish(EndReason::Cancelled, false);
            return true;
        }
        delayTimer->start(300);
        return true;
    }

    QVector<QPointer<ScreenCaptureOverlay>> overlayWindows() const
    {
        QVector<QPointer<ScreenCaptureOverlay>> windows;
        windows.reserve(frames.size());
        for (const ScreenFrame &frame : frames) {
            if (frame.overlay)
                windows.append(frame.overlay);
        }
        return windows;
    }

    void grabScreens(quint64 token)
    {
        if (!isCurrent(token))
            return;
        if (!source) {
            finish(EndReason::Cancelled, false);
            return;
        }
        if (frames.isEmpty()) {
            fail(QStringLiteral("未找到可截图的屏幕。"));
            return;
        }

        QPointer<ScreenCaptureController> owner(q);
        // 先冻结所有屏幕，再创建和显示覆盖层，避免把覆盖层截进另一块屏幕。
        for (int i = 0; i < frames.size(); ++i) {
            if (!isCurrent(token) || !source)
                return;
            const QPointer<QScreen> screen = frames[i].screen;
            if (!screen) {
                finish(EndReason::Cancelled);
                return;
            }
            const QRect geometry = screen->geometry();
            if (geometry.width() <= 0 || geometry.height() <= 0) {
                fail(QStringLiteral("屏幕尺寸无效，无法截图。"));
                return;
            }
            QImage image = screen->grabWindow(0).toImage();
            if (!owner || !isCurrent(token) || !source)
                return;
            if (!screen) {
                finish(EndReason::Cancelled);
                return;
            }
            if (image.isNull() || image.width() <= 0 || image.height() <= 0) {
                fail(QStringLiteral("读取屏幕图像失败，请稍后重试。"));
                return;
            }
            image.setDevicePixelRatio(1.0);
            frames[i].geometry = geometry;
            frames[i].image = image;
        }

        for (int i = 0; i < frames.size(); ++i) {
            if (!isCurrent(token) || !source)
                return;
            const QPointer<QScreen> screen = frames[i].screen;
            const QRect geometry = frames[i].geometry;
            if (!screen) {
                finish(EndReason::Cancelled);
                return;
            }
            // 顶层覆盖层不依赖隐藏源窗口的可见性，由控制器统一隐藏并延迟销毁。
            auto *overlay = new ScreenCaptureOverlay(frames[i].image);
            frames[i].overlay = overlay;
            const QPointer<ScreenCaptureOverlay> guardedOverlay(overlay);
            connections.append(connect(overlay, &ScreenCaptureOverlay::selectionStarted, session,
                                       [this, token, guardedOverlay]() {
                if (isCurrent(token) && source && guardedOverlay)
                    lockScreen(guardedOverlay, token);
            }));
            connections.append(connect(overlay, &ScreenCaptureOverlay::reselectRequested, session,
                                       [this, token, guardedOverlay]() {
                if (isCurrent(token) && source && guardedOverlay && lockedOverlay == guardedOverlay)
                    reselect(guardedOverlay, token);
            }));
            connections.append(connect(overlay, &ScreenCaptureOverlay::captureRequested, session,
                                       [this, token, guardedOverlay](ScreenCaptureController::Action action) {
                if (!isCurrent(token) || !source || !guardedOverlay || lockedOverlay != guardedOverlay)
                    return;
                QImage result = guardedOverlay->resultImage();
                if (result.isNull() || result.width() <= 0 || result.height() <= 0) {
                    fail(QStringLiteral("选区图像为空，请重新截图。"));
                    return;
                }
                finish(EndReason::Captured, true, result, action);
            }));
            connections.append(connect(overlay, &ScreenCaptureOverlay::cancelRequested, session,
                                       [this, token]() {
                if (isCurrent(token))
                    finish(EndReason::Cancelled);
            }));

            overlay->winId();
            if (!owner || !isCurrent(token) || !source)
                return;
            if (!screen || !guardedOverlay) {
                finish(EndReason::Cancelled);
                return;
            }
            if (overlay->windowHandle())
                overlay->windowHandle()->setScreen(screen.data());
            if (!owner || !isCurrent(token) || !source || !guardedOverlay)
                return;
            overlay->setGeometry(geometry);
            if (!owner || !isCurrent(token) || !source)
                return;
        }

        const auto windows = overlayWindows();
        for (const auto &overlay : windows) {
            if (!owner || !isCurrent(token) || !source)
                return;
            if (!overlay)
                continue;
            overlay->show();
            if (!owner || !isCurrent(token) || !source)
                return;
            if (overlay)
                overlay->raise();
        }
        if (!owner || !isCurrent(token) || !source || windows.isEmpty() || !windows.first())
            return;
        const QPointer<ScreenCaptureOverlay> focusOverlay = windows.first();
        focusOverlay->activateWindow();
        if (owner && isCurrent(token) && source && focusOverlay)
            focusOverlay->setFocus(Qt::OtherFocusReason);
    }

    void lockScreen(const QPointer<ScreenCaptureOverlay> &selected, quint64 token)
    {
        if (!isCurrent(token) || !source || !selected || (lockedOverlay && lockedOverlay != selected))
            return;
        lockedOverlay = selected;
        const auto windows = overlayWindows();
        QPointer<ScreenCaptureController> owner(q);
        for (const auto &overlay : windows) {
            if (!owner || !isCurrent(token) || !source)
                return;
            if (overlay && overlay != selected)
                overlay->hide();
        }
        if (!owner || !isCurrent(token) || !source || !selected)
            return;
        selected->raise();
        if (!owner || !isCurrent(token) || !source || !selected)
            return;
        selected->activateWindow();
        if (owner && isCurrent(token) && source && selected)
            selected->setFocus(Qt::MouseFocusReason);
    }

    void reselect(const QPointer<ScreenCaptureOverlay> &selected, quint64 token)
    {
        if (!isCurrent(token) || !source || !selected)
            return;
        lockedOverlay.clear();
        const auto windows = overlayWindows();
        QPointer<ScreenCaptureController> owner(q);
        for (const auto &overlay : windows) {
            if (!owner || !isCurrent(token) || !source)
                return;
            if (overlay)
                overlay->resetSelection();
        }
        for (const auto &overlay : windows) {
            if (!owner || !isCurrent(token) || !source)
                return;
            if (overlay)
                overlay->show();
        }
        if (!owner || !isCurrent(token) || !source || !selected)
            return;
        selected->raise();
        if (!owner || !isCurrent(token) || !source || !selected)
            return;
        selected->activateWindow();
        if (owner && isCurrent(token) && source && selected)
            selected->setFocus(Qt::OtherFocusReason);
    }

    void fail(const QString &message)
    {
        finish(EndReason::Error, true, QImage(), ScreenCaptureController::Copy, message);
    }

    static void restoreWindow(const QPointer<ScreenCaptureController> &owner,
                              const QPointer<QWidget> &window, const QByteArray &geometry,
                              Qt::WindowStates state, bool visible, quint64 token)
    {
        const auto mayRestore = [&owner, &window, token]() {
            return owner && window && owner->d->ending && owner->d->generation == token
                && !owner->d->sourceClosed;
        };
        if (!mayRestore())
            return;
        if (!geometry.isEmpty())
            window->restoreGeometry(geometry);
        if (!mayRestore())
            return;
        window->setWindowState(state);
        if (!mayRestore())
            return;
        if (!visible) {
            window->hide();
            return;
        }
        window->show();
        if (!mayRestore() || !window->isVisible())
            return;
        window->raise();
        if (mayRestore() && window->isVisible())
            window->activateWindow();
    }

    void finish(EndReason reason, bool restoreSource = true, QImage result = QImage(),
                ScreenCaptureController::Action action = ScreenCaptureController::Copy,
                QString message = QString())
    {
        if (!active || ending)
            return;
        ending = true;
        active = false;
        const quint64 completedToken = ++generation;
        const QPointer<ScreenCaptureController> owner(q);
        const QPointer<QWidget> originalSource = source;
        const QByteArray geometry = savedGeometry;
        const Qt::WindowStates state = savedState;
        const bool visible = wasVisible;
        const QVector<ScreenFrame> oldFrames = std::move(frames);
        frames.clear();
        lockedOverlay.clear();
        savedGeometry.clear();
        savedState = Qt::WindowNoState;
        wasVisible = false;
        if (delayTimer)
            delayTimer->stop();
        delayTimer = nullptr;
        for (const QMetaObject::Connection &connection : connections)
            QObject::disconnect(connection);
        connections.clear();
        if (session)
            session->deleteLater();
        session = nullptr;
        if (reason == EndReason::Silent) {
            if (originalSource)
                originalSource->removeEventFilter(this);
            source.clear();
        }

        // 不在 overlay 的鼠标/按钮事件栈中删除发送者；隐藏后再恢复源窗口。
        for (const ScreenFrame &frame : oldFrames) {
            if (frame.overlay) {
                frame.overlay->hide();
                if (frame.overlay)
                    frame.overlay->deleteLater();
            }
        }
        if (reason == EndReason::Silent)
            return;
        if (restoreSource)
            restoreWindow(owner, originalSource, geometry, state, visible, completedToken);
        if (!owner || owner->d->generation != completedToken || !owner->d->ending)
            return;
        if (originalSource)
            originalSource->removeEventFilter(owner->d.get());
        if (!originalSource || owner->d->sourceClosed)
            reason = EndReason::Cancelled;
        owner->d->source.clear();
        owner->d->sourceClosed = false;
        owner->d->ending = false;

        switch (reason) {
        case EndReason::Captured:
            emit owner->captured(result, action);
            break;
        case EndReason::Cancelled:
            emit owner->cancelled();
            break;
        case EndReason::Error:
            emit owner->error(message);
            break;
        case EndReason::Silent:
            break;
        }
    }

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (watched == source.data() && event->type() == QEvent::Close) {
            const QPointer<QObject> watchedGuard(watched);
            sourceClosed = true;
            if (active && !ending)
                finish(EndReason::Cancelled, false);
            return watchedGuard.isNull();
        }
        return QObject::eventFilter(watched, event);
    }

    ScreenCaptureController *q;
    bool active = false;
    bool ending = false;
    quint64 generation = 0;
    QPointer<QWidget> source;
    QByteArray savedGeometry;
    Qt::WindowStates savedState = Qt::WindowNoState;
    bool wasVisible = false;
    bool sourceClosed = false;
    QObject *session = nullptr;
    QTimer *delayTimer = nullptr;
    QVector<QMetaObject::Connection> connections;
    QVector<ScreenFrame> frames;
    QPointer<ScreenCaptureOverlay> lockedOverlay;
};

ScreenCaptureController::ScreenCaptureController(QObject *parent)
    : QObject(parent), d(new Private(this))
{
}

ScreenCaptureController::~ScreenCaptureController() = default;

bool ScreenCaptureController::isActive() const
{
    return d->active;
}

bool ScreenCaptureController::start(QWidget *sourceWindow)
{
    return d->start(sourceWindow);
}

void ScreenCaptureController::cancel()
{
    d->finish(Private::EndReason::Cancelled);
}
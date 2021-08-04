#include "frameless.h"

FrameLess::FrameLess(QWidget *target) :
	_target(target),
	_cursorchanged(false),
	_leftButtonPressed(false),
	_borderWidth(5)
{
	_target->setAttribute(Qt::WA_Hover);
	//auto childList = _target->findChildren<QWidget*>();
	//for (auto child : childList) {
	//	child->installEventFilter(this);
	//}
	_target->installEventFilter(this);
	_rubberband = new DragShadow();
}

FrameLess::~FrameLess()
{
	if (_rubberband) {
		delete _rubberband;
		_rubberband = nullptr;
	}
}

bool FrameLess::eventFilter(QObject *o, QEvent*e) {
	auto type = e->type();
	switch (type) {
	case QEvent::MouseMove: {
		mouseMove(static_cast<QMouseEvent*>(e));
		break;
	}
	case QEvent::HoverMove: {
		mouseHover(static_cast<QHoverEvent*>(e));
		break;
	}
	case QEvent::Leave: {
		mouseLeave(e);
		break;
	}
	case QEvent::MouseButtonPress: {
		mousePress(static_cast<QMouseEvent*>(e));
		break;
	}
	case QEvent::MouseButtonRelease: {
		mouseRealese(static_cast<QMouseEvent*>(e));
		break;
	}
	default: {
		return false;
	}
	}
	//不是改变大小事件时返回子窗口事件
	if (!_mousePress.testFlag(Edge::None)) {
		return true;
	}
	else {
		return false;
	}
}

void FrameLess::mouseHover(QHoverEvent *e) {
	updateCursorShape(_target->mapToGlobal(e->pos()));
}

void FrameLess::mouseLeave(QEvent *e) {
	if (!_leftButtonPressed) {
		_target->unsetCursor();
	}
}

void FrameLess::mousePress(QMouseEvent *e) {
	if (e->button() & Qt::LeftButton) {
		_rubberband->setGeometry(_target->frameGeometry());
		_originRect = _target->frameGeometry();
		_leftButtonPressed = true;
		calculateCursorPosition(e->globalPos(), _target->frameGeometry(), _mousePress);
	}
}

void FrameLess::mouseRealese(QMouseEvent *e) {
	if (e->button() & Qt::LeftButton) {
		_leftButtonPressed = false;
		if (_rubberband && _rubberband->isVisible())
		{
			_rubberband->hide();
			_target->setGeometry(_rubberband->geometry());
		}
	}
}

void FrameLess::mouseMove(QMouseEvent *e) {
	if (_leftButtonPressed) {
		if (!_mousePress.testFlag(Edge::None) && !_target->isMaximized()) { //_target prevent resizing if it is maximized
			QRect newRect = _originRect;
			switch (_mousePress) {
			case Edge::Top:
				newRect.setTop(e->globalPos().y());
				if (newRect.height() < _target->minimumHeight()) {
					newRect.setTop(_originRect.bottom() - _target->minimumHeight());
					newRect.setHeight(_target->minimumHeight());
				}
				break;
			case Edge::Bottom:
				newRect.setBottom(e->globalPos().y());
				if (newRect.height() < _target->minimumHeight()) {
					newRect.setBottom(_originRect.top() + _target->minimumHeight());
				}
				break;
			case Edge::Left:
				newRect.setLeft(e->globalPos().x());
				if (newRect.width() < _target->minimumWidth()) {
					newRect.setLeft(_originRect.right() - _target->minimumWidth());
					newRect.setWidth(_target->minimumWidth());
				}
				break;
			case Edge::Right:
				newRect.setRight(e->globalPos().x());
				if (newRect.width() < _target->minimumWidth()) {
					newRect.setRight(_originRect.left() + _target->minimumWidth());
				}
				break;
			case Edge::TopLeft:
				newRect.setTop(e->globalPos().y());
				if (newRect.height() < _target->minimumHeight()) {
					newRect.setTop(_originRect.bottom() - _target->minimumHeight());
					newRect.setHeight(_target->minimumHeight());
				}
				newRect.setLeft(e->globalPos().x());
				if (newRect.width() < _target->minimumWidth()) {
					newRect.setLeft(_originRect.right() - _target->minimumWidth());
					newRect.setWidth(_target->minimumWidth());
				}
				break;
			case Edge::TopRight:
				newRect.setRight(e->globalPos().x());
				if (newRect.width() < _target->minimumWidth()) {
					newRect.setRight(_originRect.left() + _target->minimumWidth());
				}
				newRect.setTop(e->globalPos().y());
				if (newRect.height() < _target->minimumHeight()) {
					newRect.setTop(_originRect.bottom() - _target->minimumHeight());
					newRect.setHeight(_target->minimumHeight());
				}
				break;
			case Edge::BottomLeft:
				newRect.setBottom(e->globalPos().y());
				if (newRect.height() < _target->minimumHeight()) {
					newRect.setBottom(_originRect.top() + _target->minimumHeight());
				}
				newRect.setLeft(e->globalPos().x());
				if (newRect.width() < _target->minimumWidth()) {
					newRect.setLeft(_originRect.right() - _target->minimumWidth());
					newRect.setWidth(_target->minimumWidth());
				}
				break;
			case Edge::BottomRight:
				newRect.setBottom(e->globalPos().y());
				if (newRect.height() < _target->minimumHeight()) {
					newRect.setBottom(_originRect.top() + _target->minimumHeight());
				}
				newRect.setRight(e->globalPos().x());
				if (newRect.width() < _target->minimumWidth()) {
					newRect.setRight(_originRect.left() + _target->minimumWidth());
				}
				break;
			default:
				return;
			}
			_rubberband->setGeometry(newRect);
			_rubberband->show();
		}
	}
	else {
		updateCursorShape(e->globalPos());
	}
}

void FrameLess::updateCursorShape(const QPoint &pos) {
	if (_target->isFullScreen() || _target->isMaximized()) {
		if (_cursorchanged) {
			_target->unsetCursor();
		}
		return;
	}
	if (!_leftButtonPressed) {
		calculateCursorPosition(pos, _target->frameGeometry(), _mouseMove);
		_cursorchanged = true;
		if (_mouseMove.testFlag(Edge::Top) || _mouseMove.testFlag(Edge::Bottom)) {
			_target->setCursor(Qt::SizeVerCursor);
		}
		else if (_mouseMove.testFlag(Edge::Left) || _mouseMove.testFlag(Edge::Right)) {
			_target->setCursor(Qt::SizeHorCursor);
		}
		else if (_mouseMove.testFlag(Edge::TopLeft) || _mouseMove.testFlag(Edge::BottomRight)) {
			_target->setCursor(Qt::SizeFDiagCursor);
		}
		else if (_mouseMove.testFlag(Edge::TopRight) || _mouseMove.testFlag(Edge::BottomLeft)) {
			_target->setCursor(Qt::SizeBDiagCursor);
		}
		else if (_cursorchanged) {
			_target->unsetCursor();
			_cursorchanged = false;
		}
	}
}

void FrameLess::calculateCursorPosition(const QPoint &pos, const QRect &framerect, Edges &_edge) {
	bool onLeft = pos.x() >= framerect.x() - _borderWidth && pos.x() <= framerect.x() + _borderWidth &&
		pos.y() <= framerect.y() + framerect.height() - _borderWidth && pos.y() >= framerect.y() + _borderWidth;

	bool onRight = pos.x() >= framerect.x() + framerect.width() - _borderWidth && pos.x() <= framerect.x() + framerect.width() &&
		pos.y() >= framerect.y() + _borderWidth && pos.y() <= framerect.y() + framerect.height() - _borderWidth;

	bool onBottom = pos.x() >= framerect.x() + _borderWidth && pos.x() <= framerect.x() + framerect.width() - _borderWidth  &&
		pos.y() >= framerect.y() + framerect.height() - _borderWidth && pos.y() <= framerect.y() + framerect.height();

	bool onTop = pos.x() >= framerect.x() + _borderWidth && pos.x() <= framerect.x() + framerect.width() - _borderWidth &&
		pos.y() >= framerect.y() && pos.y() <= framerect.y() + _borderWidth;

	bool  onBottomLeft = pos.x() <= framerect.x() + _borderWidth && pos.x() >= framerect.x() &&
		pos.y() <= framerect.y() + framerect.height() && pos.y() >= framerect.y() + framerect.height() - _borderWidth;

	bool onBottomRight = pos.x() >= framerect.x() + framerect.width() - _borderWidth && pos.x() <= framerect.x() + framerect.width() &&
		pos.y() >= framerect.y() + framerect.height() - _borderWidth && pos.y() <= framerect.y() + framerect.height();

	bool onTopRight = pos.x() >= framerect.x() + framerect.width() - _borderWidth && pos.x() <= framerect.x() + framerect.width() &&
		pos.y() >= framerect.y() && pos.y() <= framerect.y() + _borderWidth;

	bool onTopLeft = pos.x() >= framerect.x() && pos.x() <= framerect.x() + _borderWidth &&
		pos.y() >= framerect.y() && pos.y() <= framerect.y() + _borderWidth;

	if (onLeft) {
		_edge = Left;
	}
	else if (onRight) {
		_edge = Right;
	}
	else if (onBottom) {
		_edge = Bottom;
	}
	else if (onTop) {
		_edge = Top;
	}
	else if (onBottomLeft) {
		_edge = BottomLeft;
	}
	else if (onBottomRight) {
		_edge = BottomRight;
	}
	else if (onTopRight) {
		_edge = TopRight;
	}
	else if (onTopLeft) {
		_edge = TopLeft;
	}
	else {
		_edge = None;
	}
}
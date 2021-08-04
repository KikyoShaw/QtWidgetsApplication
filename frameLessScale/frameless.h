#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QRubberBand>
#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QRect>
#include <QtCore/QPoint>
#include <QtCore/Qt>
#include <QtGui/QHoverEvent>
#include <QtGui/QMouseEvent>
#include "DragShadow.h"

class FrameLess : public QObject 
{
	Q_OBJECT

public:
	enum Edge {
		None = 0x0,
		Left = 0x1,
		Top = 0x2,
		Right = 0x4,
		Bottom = 0x8,
		TopLeft = 0x10,
		TopRight = 0x20,
		BottomLeft = 0x40,
		BottomRight = 0x80,
	};
	Q_ENUM(Edge);
	Q_DECLARE_FLAGS(Edges, Edge);

	FrameLess(QWidget *target);
	~FrameLess();
protected:
	bool eventFilter(QObject *o, QEvent *e) override;
	void mouseHover(QHoverEvent*);
	void mouseLeave(QEvent*);
	void mousePress(QMouseEvent*);
	void mouseRealese(QMouseEvent*);
	void mouseMove(QMouseEvent*);
	void updateCursorShape(const QPoint &);
	void calculateCursorPosition(const QPoint &, const QRect &, Edges &);

private:
	QWidget *_target = nullptr;
	DragShadow *_rubberband = nullptr;
	bool _cursorchanged;
	bool _leftButtonPressed;
	Edges _mousePress = Edge::None;
	Edges _mouseMove = Edge::None;
	int _borderWidth;
	int maximizeWidth;
	QRect _originRect;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(FrameLess::Edges);
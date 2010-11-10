#ifndef OPPOSITEFINDER_H
#define OPPOSITEFINDER_H
#include <QtGui>
#include <QGraphicsObject>
#include "src/defines.h"
class Element;

class OppositeFinder : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit OppositeFinder(Element *eparent, int elementType, QGraphicsObject *parent = 0);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundingRect() const;
	OppositeFinder *otherOppositeFinder();
	Element *otherElement();
	Element *element();
	QPainterPath shape() const;
	void setEntrance(ConnectionType type);
signals:
	void foundOther(Element *);
public slots:
private:
	OppositeFinder *myOther;
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	bool pressed;
	Element *myParentElement;
	int myElementType;
	QGraphicsLineItem *myLine;
	ConnectionType myEntrance;
	QColor myColor;
	QColor findNewColor();
	static QList<QColor> colors;
	void makeConnection(OppositeFinder * other);
};

#endif // OPPOSITEFINDER_H

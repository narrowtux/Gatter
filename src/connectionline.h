#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include <QGraphicsLineItem>
#include "src/defines.h"

class ConnectionLine : public QGraphicsLineItem
{
public:
    explicit ConnectionLine(QGraphicsItem *parent = 0);
	QRectF boundingRect() const;
	void setConnectionTypes(ConnectionType p1, ConnectionType p2);
	QPainterPath shape() const;
signals:

public slots:
private:
	QList<QLineF> getLines();
	QPolygonF polygonFromLine(QLineF line, qreal radius=5);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	ConnectionType t1,t2;
};

#endif // CONNECTIONLINE_H

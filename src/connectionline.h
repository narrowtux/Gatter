#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include <QGraphicsLineItem>
#include "src/defines.h"

class Connection;

class ConnectionLine : public QGraphicsLineItem
{
public:
    explicit ConnectionLine(QGraphicsItem *parent = 0);
	QRectF boundingRect() const;
	void setConnectionTypes(ConnectionType p1, ConnectionType p2);
	virtual QPainterPath shape() const;
	void setConnections(Connection* co1, Connection* co2);
	Connection* connection1();
	Connection* connection2();
signals:

public slots:
private:
	Connection *c1, *c2;
	QList<QLineF> getLines();
	QPolygonF polygonFromLine(QLineF line, qreal radius = 20);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	ConnectionType t1,t2;
};

#endif // CONNECTIONLINE_H

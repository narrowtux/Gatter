#ifndef TUNNEL_H
#define TUNNEL_H

#include <QtGui>
#include "element.h"
#include "oppositefinder.h"

class Tunnel : public Element
{
    Q_OBJECT
public:
    explicit Tunnel(QGraphicsObject *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

private slots:
	void setEntranceTypeInt(int type);
private:
	ConnectionType myEntrance;
	Tunnel *myOther;
	OppositeFinder *myOppositeFinder;
	void recalculate();
	void setEntranceType(ConnectionType t);
	bool createFormBefore();
};

#endif // TUNNEL_H

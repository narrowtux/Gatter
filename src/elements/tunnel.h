#ifndef TUNNEL_H
#define TUNNEL_H

#include <QtGui>
#include "element.h"
#include "oppositefinder.h"
#include <QVector>

class Tunnel : public Element
{
    Q_OBJECT
public:
    explicit Tunnel(QGraphicsObject *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	ConnectionType entranceType();
	Tunnel *tunnelExit();
	void setPrivateXml(QXmlStreamWriter *xml);
	void readPrivateXml(QXmlStreamReader *xml);
	OppositeFinder * oppositeFinder();
signals:

private slots:
	void setEntranceTypeInt(int type);
	void onOtherFound(Element *element);
private:
	ConnectionType myEntrance;
	Tunnel *myOther;
	OppositeFinder *myOppositeFinder;
	QVector<bool> myValues;
	void recalculate();
	void setEntranceType(ConnectionType t);
	bool createFormBefore();
	void connectionsChanged();
	bool resizing;
};

#endif // TUNNEL_H

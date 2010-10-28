#include "connectionline.h"
#include <QPainter>
#include <QDebug>
#include "src/defines.h"
ConnectionLine::ConnectionLine(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
	setData(ElementRecognition, "connectionline");
}

void ConnectionLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option)
	Q_UNUSED(widget)
	if((t1==Input&&line().x1()<line().x2())||(t1==Output&&line().x1()>line().x2())){
		QLineF line1(line().x1(), line().y1() ,line().x1(),line().y1()+qRound(line().dy()/2));
		painter->drawLine(line1);
		QLineF line2(line1.p2(), QPointF(line().p2().x(), line1.p2().y()));
		painter->drawLine(line2);
		QLineF line3(line2.p2(), line().p2());
		painter->drawLine(line3);
	} else {
		QLineF line1(line().x1(), line().y1() ,line().x1()+qRound(line().dx()/2),line().y1());
		painter->drawLine(line1);
		QLineF line2(line1.p2(), QPointF(line1.p2().x(), line1.p2().y()+ line().dy()));
		painter->drawLine(line2);
		QLineF line3(line2.p2(), line().p2());
		painter->drawLine(line3);
	}
}

QRectF ConnectionLine::boundingRect() const {
	return QGraphicsLineItem::boundingRect();
}

bool ConnectionLine::pointOnLine(QPointF point, qreal radius){
	QLineF line1(line().x1(), line().y1() ,line().x1()+qRound(line().dx()/2),line().y1());
	QLineF line2(line1.p2(), QPointF(line1.p2().x(), line1.p2().y()+ line().dy()));
	QLineF line3(line2.p2(), line().p2());
	QList<QLineF> lines;
	lines<<line1<<line2<<line3;
	foreach(QLineF line, lines){
		QRectF rect;
		rect.setTopLeft(line.p1());
		rect.setBottomRight(line.p2());
		rect.setWidth(qMax(radius, rect.width()));
		rect.setHeight(qMax(radius, rect.height()));
		if(rect.contains(point)){
			return true;
		}
	}
	return false;
}

void ConnectionLine::setConnectionTypes(ConnectionType p1, ConnectionType p2){
	t1=p1;
	t2=p2;
	update();
}

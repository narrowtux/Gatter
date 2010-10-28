#include "connectionline.h"
#include <QPainter>
#include <QDebug>
ConnectionLine::ConnectionLine(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
	
}

void ConnectionLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option)
	Q_UNUSED(widget)
	qDebug()<<line().dx();
	QLineF line1(line().x1(), line().y1() ,line().x1()+qRound(line().dx()/2),line().y1());
	painter->drawLine(line1);
	QLineF line2(line1.p2(), QPointF(line1.p2().x(), line1.p2().y()+ line().dy()));
	painter->drawLine(line2);
	QLineF line3(line2.p2(), line().p2());
	painter->drawLine(line3);
}

QRectF ConnectionLine::boundingRect() const {
	return QGraphicsLineItem::boundingRect();
}

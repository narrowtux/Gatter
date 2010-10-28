#include "connectionline.h"
#include <QPainter>
#include <QDebug>
#include "src/defines.h"
ConnectionLine::ConnectionLine(QGraphicsItem *parent) :
    QGraphicsLineItem(parent)
{
	setData(ElementRecognition, QVariant("connectionline"));
}

void ConnectionLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setPen(pen());
	QList<QLineF> lines=getLines();
	foreach(QLineF l, lines){
		painter->drawLine(l);
	}
}

QRectF ConnectionLine::boundingRect() const {
	return QGraphicsLineItem::boundingRect();
}

void ConnectionLine::setConnectionTypes(ConnectionType p1, ConnectionType p2){
	t1=p1;
	t2=p2;
	update();
}

QPainterPath ConnectionLine::shape() const{
	QPainterPath ret;
//	ret.addRect(boundingRect());
	QList<QLineF> lines=const_cast<ConnectionLine*>(this)->getLines();
	foreach(QLineF l, lines){
		ret.addPolygon(const_cast<ConnectionLine*>(this)->polygonFromLine(l));
	}
	return ret;
}

QList<QLineF> ConnectionLine::getLines(){
	QList<QLineF> ret;
	if((t1==Input&&line().x1()<line().x2())||(t1==Output&&line().x1()>line().x2())){
		QLineF line1(line().x1(), line().y1() ,line().x1(),line().y1()+qRound(line().dy()/2));
		QLineF line2(line1.p2(), QPointF(line().p2().x(), line1.p2().y()));
		QLineF line3(line2.p2(), line().p2());
		ret<<line1<<line2<<line3;
	} else {
		QLineF line1(line().x1(), line().y1() ,line().x1()+qRound(line().dx()/2),line().y1());
		QLineF line2(line1.p2(), QPointF(line1.p2().x(), line1.p2().y()+ line().dy()));
		QLineF line3(line2.p2(), line().p2());
		ret<<line1<<line2<<line3;
	}
	return ret;
}

QPolygonF ConnectionLine::polygonFromLine(QLineF l, qreal radius){
	QPolygonF ret;
	QLineF top, left, right;
	top=QLineF::fromPolar(radius/2,l.angle()-90);
	//top.setP1(line().p1());
	qDebug()<<top<<l.angle();
	right.setP1(top.p2()+l.p1());
	right.setAngle(l.angle());
	right.setLength(l.length());
	left.setP1(l.p2()-top.p2());
	left.setAngle(l.angle()+180);
	left.setLength(l.length());
	ret<<right.p1()<<right.p2()<<left.p1()<<left.p2();
	return ret;
}

#include "connection.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>
Connection::Connection(QObject *parent) :
    QObject(parent)
{
    myValue=Low;
    lastValue=Low;
    myNegated=false;
    myConnectionType=Output;
    setData(ElementRecognition,QVariant("Connection"));
    setFlag(ItemSendsScenePositionChanges,true);
    connectedTo=0;
    lastI=0;
    line=0;
    poked=false;
}

void Connection::setValue(bool v)
{
    myValue=v;
    if(myValue!=lastValue){
	bool endValue=myValue;
	if(myNegated){
	    endValue=!endValue;
	}
	emit(changed(endValue));
	if(connectedTo!=0&&myConnectionType==Output){
	    qDebug()<<"Connection Value Changed.";
	    if(endValue){
		connectedTo->setValue(High);
	    } else {
		connectedTo->setValue(Low);
	    }
	}
    }
    lastValue=myValue;
    update();
}

void Connection::setNegated(bool n)
{
    myNegated=n;
}

bool Connection::isNegated()
{
    return myNegated;
}

bool Connection::value()
{
    if(myNegated)
	return !myValue;
    else
	return myValue;
}

QRectF Connection::boundingRect() const{
    return QRectF(0,-5,20,10);
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QColor("white"));
    switch(myValue){
    case High:
	painter->setPen(QColor("red"));
	break;
    case Low:
	painter->setPen(QColor("black"));
	break;
    }

    
    switch(myConnectionType){
    case Input:
	if(isNegated()){
	    painter->drawEllipse(QPointF(15,0),5,5);
	    painter->drawLine(QPointF(0,0),QPointF(10,0));
	}else {
	    painter->drawLine(QPointF(0,0),QPointF(20,0));
	}
	if(poked){
	    painter->setBrush(QColor("gray"));
	    painter->setPen(Qt::NoPen);
	    painter->drawEllipse(QPointF(3,0),3,3);
	}
	break;
    case Output:
	if(isNegated()){
	    painter->drawEllipse(QPointF(5,0),5,5);
	    painter->drawLine(QPointF(10,0),QPointF(20,0));
	}else {
	    painter->drawLine(QPointF(0,0),QPointF(20,0));
	}
	if(poked){
	    painter->setBrush(QColor("gray"));
	    painter->setPen(Qt::NoPen);
	    painter->drawEllipse(QPointF(17,0),3,3);
	}
	break;
    }
}

void Connection::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::LeftButton){
	if(connectedTo!=0){
	    connectedTo->connectedTo=0;
	    connectedTo->line=0;
	    connectedTo->update();
	    delete line;
	    connectedTo=0;
	    update();
	}
	line=scene()->addLine(QLineF(event->scenePos(),event->scenePos()));
	startPos=event->scenePos();
	qDebug()<<event->scenePos();
	pressed=1;
	poke(true);
    }
}

void Connection::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(pressed){
	QLineF l(startPos,event->scenePos());
	qreal ln=l.length()-1;
	l.setLength(ln);
	line->setLine(l);
	QGraphicsItem* i=scene()->itemAt(event->scenePos());
	if(i!=0&&i->data(ElementRecognition).toString()=="Connection"){
	    ((Connection*)i)->poke(true);
	    lastI=(Connection*)i;
	}else if(lastI!=0){
	    lastI->poke(false);
	    lastI=0;
	}
    }
}

void Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::LeftButton){
	QGraphicsItem* i=scene()->itemAt(event->scenePos());
	line->setLine(QLineF(startPos,event->scenePos()));
	qDebug()<<event->scenePos();
	if(i->data(ElementRecognition).toString()=="Connection"&&i!=this){
	    Connection* connection=static_cast<Connection*>(i);
	    if(connection->myConnectionType!=myConnectionType){
		connection->setOther(this);
		connection->poke(false);
		updateLine();
	    } else {
		scene()->removeItem(line);
		delete line;
	    }
	} else {
	    scene()->removeItem(line);
	    delete line;
	}
	poke(false);
	pressed=0;
    }
}

void Connection::setOther(Connection *other){
    if(connectedTo!=0){
	connectedTo->line=0;
	connectedTo->connectedTo=0;
	connectedTo->update();
	delete line;
	line=0;
    }
    connectedTo=other;
    if(other!=0){
	line=other->line;
	connectedTo->connectedTo=this;
	if(myConnectionType==Input)
	    setValue(connectedTo->value());
	else
	    connectedTo->setValue(myValue);
    }
    update();
}

QVariant Connection::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change==ItemScenePositionHasChanged){
	updateLine();
    }
    return value;
}

void Connection::poke(bool in){
    poked=in;
    update();
}

void Connection::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    poke(true);
}

void Connection::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    poke(false);
}

void Connection::updateLine(){
    if(connectedTo!=0){
	QPointF p1,p2;
	switch(myConnectionType){
	case Input:
	    p1=scenePos();
	    p2=connectedTo->scenePos()+QPointF(20,0);
	    break;
	case Output:
	    p1=scenePos()+QPointF(20,0);
	    p2=connectedTo->scenePos();
	    break;
	}

	line->setLine(QLineF(p1,p2));
    }
}

Connection::~Connection()
{
    if(connectedTo!=0){
	    connectedTo->setOther(0);
    }
}

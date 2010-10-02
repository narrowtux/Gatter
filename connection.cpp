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
    setFlags(ItemSendsScenePositionChanges);
    myConnectedTo=0;
    lastI=0;
    line=0;
    poked=false;
    setAcceptHoverEvents(true);
    label=new QGraphicsTextItem(this);
}

bool Connection::isConnected()
{
    return myConnectedTo!=0;
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
	if(myConnectedTo!=0&&myConnectionType==Output){
	    qDebug()<<"Connection Value Changed to"<<endValue;
	    if(endValue){
		myConnectedTo->setValue(High);
		line->setPen(QColor("red"));
	    } else {
		myConnectedTo->setValue(Low);
		line->setPen(QColor("black"));
	    }
	}
    }
    lastValue=myValue;
    update();
}

void Connection::setNegated(bool n)
{
    bool last=myNegated;
    myNegated=n;
    emit(recalculate());
    if(last!=myNegated){
	//setValue(!myValue);
    }
    update();
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
    return QRectF(0,-10,20,20);
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setBrush(QColor("white"));
    if(myValue&&!myNegated||!myValue&&myNegated){
	painter->setPen(QColor("red"));
//	if(line!=0){
//	    line->setPen(QColor("red"));
//	}
    } else {
	painter->setPen(QColor("black"));
//	if(line!=0){
//	    line->setPen(QColor("black"));
//	}
    }

    QRectF text;
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
	label->setPos(20,-10);
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
	label->setPos(-10,-10);
	break;
    }
}

void Connection::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::LeftButton){
	if(myConnectedTo!=0){
	    myConnectedTo->myConnectedTo=0;
	    myConnectedTo->line=0;
	    myConnectedTo->update();
	    delete line;
	    myConnectedTo=0;
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
    if(myConnectedTo!=0){
	myConnectedTo->line=0;
	myConnectedTo->myConnectedTo=0;
	myConnectedTo->update();
	delete line;
	line=0;
    }
    myConnectedTo=other;
    if(other!=0){
	line=other->line;
	myConnectedTo->myConnectedTo=this;
	if(myConnectionType==Input){
	    if(myConnectedTo->myNegated)
		setValue(!myConnectedTo->value());
	    else
		setValue(myConnectedTo->value());
	}else{
	    if(myNegated)
		myConnectedTo->setValue(!myValue);
	    else
		myConnectedTo->setValue(myValue);
	}
    }
    update();
}

QVariant Connection::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change==ItemScenePositionHasChanged||change==ItemPositionHasChanged){
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
    if(myConnectedTo!=0){
	QPointF p1,p2;
	switch(myConnectionType){
	case Input:
	    p1=scenePos();
	    p2=myConnectedTo->scenePos()+QPointF(20,0);
	    break;
	case Output:
	    p1=scenePos()+QPointF(20,0);
	    p2=myConnectedTo->scenePos();
	    break;
	}

	line->setLine(QLineF(p1,p2));
    }
}

Connection::~Connection()
{
    if(myConnectedTo!=0){
	    myConnectedTo->setOther(0);
    }
}

void Connection::setName(QString name){
    myName=name;
    QFont f;
    if(name.contains('*')){
	f.setOverline(true);
	name.replace('*',"");
    }
    label->setFont(f);
    label->setPlainText(name);

}

QString Connection::name(){
    return myName;
}

void Connection::setElement(Element *e){
    myElement=e;
}

Element* Connection::element(){
    return myElement;
}

Connection* Connection::connectedTo(){
    return myConnectedTo;
}

void Connection::connectWith(Connection *c){
    line=new QGraphicsLineItem(0,scene());
    c->setOther(this);
    updateLine();
}

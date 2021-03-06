#include "connection.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>

#include "src/scene/scene.h"
#include "src/elements/element.h"
#include "src/elements/distributor.h"
#include "src/elements/tunnel.h"
Connection::Connection(QObject *parent) :
		QObject(parent)
{
    myValue=Low;
    lastValue=Low;
    myNegated=false;
    myConnectionType=Output;
    setData(ElementRecognition,QVariant("Connection"));
    setFlags(ItemSendsScenePositionChanges|ItemSendsGeometryChanges);
    myConnectedTo=0;
    lastI=0;
    line=0;
	label=0;
    poked=false;
    setAcceptHoverEvents(true);
    //label=new QGraphicsTextItem(this);
    //label->setAcceptHoverEvents(false);
    myClock=false;
	lastV=false;
	lastVInit=false;
}

void Connection::setClock(bool clock)
{
    myClock=clock;
}

bool Connection::isConnected()
{
    return myConnectedTo!=0;
}

void Connection::setValue(bool v)
{
    if(Scene::debugMethods){
		qDebug()<<(void*)this<<","<<property("setValue").toInt()<<", Connection::setValue";
		setProperty("setValue",property("setValue").toInt()+1);
    }
    myValue=v;
    if(myValue!=lastValue){
		bool endValue=myValue;
		if(myNegated){
			endValue=!endValue;
		}
		Scene* s=(Scene*)scene();
		if(s!=0&&!s->isLoading()){
			emit(changed(endValue));
			if(myConnectedTo!=0&&myConnectionType==Output){
				//qDebug()<<"Connection Value Changed to"<<endValue;
				if(endValue){
					myConnectedTo->setValue(High);
					line->setPen(Scene::highValueColor);
				} else {
					myConnectedTo->setValue(Low);
					line->setPen(QColor("black"));
				}
			}
		}
		if(myConnectionType==Input&&line!=0){
			if(endValue){
				//line->setPen(Scene::highValueColor);
			} else {
				//line->setPen(QColor(0,0,0));
			}
		}
		update();
    }
    lastValue=myValue;
}

void Connection::setNegated(bool n)
{
    bool last=myNegated;
    myNegated=n;
    emit(recalculate());
    if(last!=myNegated){
		setValue(myValue);
    }
    update();
}

bool Connection::isNegated()
{
    return myNegated;
}

bool Connection::value()
{
    return myValue!=myNegated;
}

QRectF Connection::boundingRect() const{
    return QRectF(0,-10,20,20);
}

void Connection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setBrush(QColor("white"));
    if((myValue&&!myNegated)||(!myValue&&myNegated)){
		painter->setPen(QPen(Scene::highValueColor,1));
		//	if(line!=0){
		//	    line->setPen(QColor("red"));
		//	}
    } else {
		painter->setPen(QPen(QColor("black"),1));
		//	if(line!=0){
		//	    line->setPen(QColor("black"));
		//	}
    }
	if(poked){
		if(painter->pen().color()==QColor("red")){
			painter->setPen(QPen(QColor("darkred"),1));
		} else {
			painter->setPen(QPen(QColor("lightgray"),1));
		}
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
		if(label!=0)
			label->setPos(0,-20);
		break;
    case Output:
		if(isNegated()){
			painter->drawEllipse(QPointF(5,0),5,5);
			painter->drawLine(QPointF(10,0),QPointF(20,0));
		}else {
			painter->drawLine(QPointF(0,0),QPointF(20,0));
		}
		if(label!=0)
			label->setPos(-20,-10);
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
			line=0;
			myConnectedTo=0;
			update();
		}
		if(line!=0){
			delete line;
			line=0;
		}
		line=new ConnectionLine;
		scene()->addItem(line);
		updateLine(event->scenePos());
		startPos=event->scenePos();
		qDebug()<<event->scenePos();
		pressed=1;
		poke(true);
		static_cast<Scene*>(scene())->highlight(0);
    }
}

void Connection::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    if(pressed){
		updateLine(event->scenePos());
		QGraphicsItem* i=((Scene*)scene())->itemAt(event->scenePos());
		if(i!=0&&i->data(ElementRecognition).toString()=="Connection"&&i!=this){
			if(i!=lastI&&lastI!=0&&lastI->myConnectedTo==0){
				lastI->poke(false);
				lastI->setValue(lastV);
			}
			((Connection*)i)->poke(true);
			if(lastI!=i){
				lastI=(Connection*)i;
				if(lastI->myConnectionType==Input&&myConnectionType==Output){
					lastV=lastI->myValue;
					lastI->setValue(value());
				}
			}
			if(lastI->connectionType()!=myConnectionType)
				updateLine(QPointF(), lastI);
		}else if(lastI!=0){
			lastI->poke(false);
			lastI->setValue(lastV);
			lastI=0;
		}
    } else {
		if(line!=0){
			delete line;
			line=0;
		}
	}
}

void Connection::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    if(event->button()==Qt::LeftButton){
		QGraphicsItem* i=((Scene*)scene())->itemAt(event->scenePos());
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
				line=0;
			}
		} else {
			scene()->removeItem(line);
			delete line;
			line=0;
		}
		if(i!=0&&i->data(ElementRecognition).toString()=="connectionline"&&myConnectionType==Input){
			//Connect lines
			ConnectionLine* li=static_cast<ConnectionLine*>(i);
			Scene* sc=static_cast<Scene*>(scene());
			Distributor* d=new Distributor;
			sc->addElement(d);
			Connection *input, *output;
			if(li->connection1()->myConnectionType==Input){
				input=li->connection1();
				output=li->connection2();
			} else {
				input=li->connection2();
				output=li->connection1();
			}
			QPointF pos;
			pos.setX(event->scenePos().x());
			pos.setY(output->scenePos().y());
			d->setPos(pos-QPointF(0.5,0.5));
			d->setOutputs(2);
			d->myInputs[0]->connectWith(output);
			d->myOutputs[0]->connectWith(input);
			d->myOutputs[1]->connectWith(this);
		} 
		poke(false);
		pressed=0;
    }else{
		if(line!=0){
			delete line;
			line=0;
		}
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
		emit(changed(myValue));
    }
    update();
}

QVariant Connection::itemChange(GraphicsItemChange change, const QVariant &value){
    if(change==ItemScenePositionHasChanged||change==ItemPositionHasChanged||change==ItemRotationHasChanged){
		updateLine();
    }
    return value;
}

void Connection::poke(bool in){
    poked=in;
    update();
}

void Connection::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
	Q_UNUSED(event)
    poke(true);
	if(myConnectedTo!=0){
		static_cast<Scene*>(scene())->highlight(connectedElements());
	}else
		static_cast<Scene*>(scene())->highlight(0);
}

void Connection::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
	Q_UNUSED(event)
    poke(false);
	static_cast<Scene*>(scene())->highlight(0);
}

void Connection::updateLine(QPointF toPoint, Connection*c){
	if(c==0&&myConnectedTo!=0)
		c=myConnectedTo;
    if(line!=0){
		QPointF p1,p2;
		switch(myConnectionType){
		case Input:
			p1=scenePos();
			if(c!=0)p2=c->mapToScene(QPointF(20,0));
			break;
		case Output:
			p1=mapToScene(QPointF(20,0));
			if(c!=0)p2=c->scenePos();
			break;
		}
		line->setConnectionTypes(myConnectionType,myConnectionType==Input?Output:Input);
		line->setConnections(this,c);
		if(!toPoint.isNull()){
			p2=toPoint;
		}
		QLineF l(p1,p2);
		if(!toPoint.isNull()){
			l.setLength(l.length()-2);
		}
		line->setLine(l);
	}
	if(line!=0&&myConnectedTo==0&&toPoint.isNull()&&c==0){
		delete line;
		line=0;
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
    myElement->update();
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
    if(c!=0){
		line=new ConnectionLine;
		scene()->addItem(line);
		c->setOther(this);
		updateLine();
    } else {
		setOther(0);
    }
}


void Connection::setPos(const QPointF &pos){
    qreal x=pos.x();
    qreal y=(int)pos.y();
    QGraphicsItem::setPos(x,y);
}

void Connection::setPos(qreal x, qreal y){
    setPos(QPointF(x,y));
}

ConnectionType Connection::connectionType(){
	return myConnectionType;
}

QList<QGraphicsItem *> Connection::connectedElements(QList<QGraphicsItem *> before){
	foreach(QGraphicsItem *item, before){
		if(this==item)
			return QList<QGraphicsItem*>();
	}
	before<<this;
	Element *connectedToElement;
	QList<QGraphicsItem *> ret;
	if(myConnectedTo!=0){
		connectedToElement=myConnectedTo->element();
	}else{
		return QList<QGraphicsItem*>();
	}
	ret<<connectedToElement;
	if(connectedToElement->myType=="distributor"){
		if(myConnectionType==Input){
			ret<<connectedToElement->myInputs[0]->connectedElements(before);
		} else {
			foreach(Connection *c, connectedToElement->myOutputs){
				ret<<c->connectedElements(before);	
			}
		}
		return ret;
//	} else if(connectedToElement->myType=="tunnel") {
//		Tunnel *tunnelIn, *tunnelOut;
//		tunnelIn=static_cast<Tunnel*>(connectedToElement);
//		if(tunnelIn->entranceType()==Output){
//			tunnelOut=tunnelIn;
//			tunnelIn=tunnelOut->tunnelExit();
//		}
//		ret<<tunnelIn<<tunnelOut;
//		if(myConnectionType==Output){
//			ret<<tunnelIn->myInputs[0]->connectedElements(before);
//		} else {
//			ret<<tunnelOut->myOutputs[0]->connectedElements(before);
//		}
	} else {
		return QList<QGraphicsItem *>()<<connectedToElement;
	}
}

void Connection::flushValue()
{
	emit(changed(value()));
}

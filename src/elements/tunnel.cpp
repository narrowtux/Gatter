#include "tunnel.h"
#include "src/defines.h"
Tunnel::Tunnel(QGraphicsObject *parent) :
    Element(parent)
{
	myOther=0;
	myType="tunnel";
	myOppositeFinder=new OppositeFinder(this, TunnelID, this);
	myOppositeFinder->setParentItem(this);
	myOppositeFinder->setPos(boundingRect().left()+10, boundingRect().top()+10);
	height=50;
	width=50;
	minHeight=30;
	setEntranceType(Input);
}

QRectF Tunnel::boundingRect() const
{
	return QRectF(-width/2, -height/2, width, height);
}

void Tunnel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Element::paint(painter, option, widget);
}

void Tunnel::recalculate()
{
	if(myEntrance==Output){
		myOutputs[0]->setValue(myValue);
	} else {
		myValue=myInputs[0]->value();
		if(myOppositeFinder->otherElement()!=0){
			myOther=static_cast<Tunnel*>(myOppositeFinder->otherElement());
			myOther->myValue=myValue;
			myOther->recalculate();
		}
	}
}

void Tunnel::setEntranceType(ConnectionType t)
{
	myEntrance=t;
	if(t==Input){
		setMinMaxInputsOutputs(1,1,0,0);
	} else {
		setMinMaxInputsOutputs(0,0,1,1);
	}
}

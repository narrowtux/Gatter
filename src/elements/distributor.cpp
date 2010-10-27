#include "distributor.h"

Distributor::Distributor(QGraphicsObject *parent) :
    Element(parent)
{
	width=10;
	height=10;
	setMinMaxInputsOutputs(1,1,3,3);
	myType="distributor";
}

void Distributor::relayoutConnections(){
	if(myInputs.count()==1&&myOutputs.count()==3){
		myOutputs.at(0)->setRotation(-90);
		myOutputs.at(1)->setRotation(0);
		myOutputs.at(2)->setRotation(90);
		myInputs.at(0)->setPos(-25,0);
		myOutputs.at(0)->setPos(0,-5);
		myOutputs.at(1)->setPos(5,0);
		myOutputs.at(2)->setPos(0,5);
	} else {
		Element::relayoutConnections();
	}
}

QRectF Distributor::boundingRect() const {
	return QRectF(-5,-5,width,height);
}

void Distributor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	Q_UNUSED(option)
	Q_UNUSED(widget)
	painter->setBrush(QBrush(QColor(0,0,0)));
	QPen pen(getSelectionPen());
	if(!isSelected()){
		pen.setColor(QColor(0,0,0));
	}
	painter->setPen(pen);
	painter->drawEllipse(QPointF(0,0),3,3);
}

void Distributor::recalculate(){
	if(myInputs.count()>=1){
		foreach(Connection*c, myOutputs){
			c->setValue(myInputs.at(0)->value());
		}
	}
}

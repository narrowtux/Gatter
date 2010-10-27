#include "distributor.h"

Distributor::Distributor(QGraphicsObject *parent) :
    Element(parent)
{
	width=10;
	height=10;
	setMinMaxInputsOutputs(1,1,1,8);
	setInputs(3);
	myType="distributor";
}

void Distributor::relayoutConnections(){
	QList<Connection*> allConnections=myInputs+myOutputs;
	qreal angle=360/allConnections.count();
	for(int i=0;i<allConnections.count();i++){
		Connection*c=allConnections.at(i);
		c->setRotation(180+angle*i);
		c->setTransformOriginPoint(-4,0);
		if(c->connectionType()==Input){
			c->setRotation(0);
			c->setPos(-24,0);
		} else {
			c->setPos(4,0);
		}
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

#include "delay.h"

Delay::Delay(QObject *parent) :
    Element(parent)
{
    height=30;
    width=30;
    minHeight=30;
    setMinMaxInputsOutputs(1,1,1,1);
    value=0;
    timer=new QTimer;
    timer->setInterval(100);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendChanges()));
    myType="delay";
}

void Delay::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter,option,widget);
    painter->drawText(boundingRect(),tr("Delay"));
}

QRectF Delay::boundingRect() const{
    return QRectF(0,0,width,height);
}

void Delay::recalculate(){
    if(!value){
	value=myInputs[0]->value();
	if(value){
	    myOutputs[0]->setValue(1);
	    timer->start();
	}
    }
}

void Delay::sendChanges(){
    myOutputs[0]->setValue(0);
    value=0;
}

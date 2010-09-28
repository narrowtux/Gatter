#include "lamp.h"

Lamp::Lamp(QObject *parent) :
    Element(parent)
{
    value=false;
    height=30;
    width=30;
    addInput(new Connection);
    setData(ElementName,"Lamp");
}

QRectF Lamp::boundingRect() const {
    return QRectF(-15,-15,width,height);
}

void Lamp::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    if(!isSelected()){
	painter->setPen(QColor("black"));
    } else {
	painter->setPen(getSelectionPen());
    }
    if(value){
	painter->setBrush(QColor("red"));
    }else{
	painter->setBrush(QColor("white"));
    }
    painter->drawEllipse(QPointF(0,0),13,13);
}

void Lamp::recalculate(){
    value=myInputs[0]->value();
    qDebug()<<"Recalculated";
    update();
}

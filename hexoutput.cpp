#include "hexoutput.h"

HexOutput::HexOutput(QGraphicsObject *parent) :
    Element(parent)
{
    width=50;
    height=50;
    minHeight=50;
    setMinMaxInputsOutputs(7,7,0,0);
    for (int i=0;i<7;i++){
	values<<false;
    }
    myType="hexoutput";
}

void HexOutput::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter,option,widget);
    QPen onPen("red");
    onPen.setWidth(3);
    onPen.setCapStyle(Qt::RoundCap);
    QPen offPen("gray");
    /*
     Seven-Segment Display:
        A0
     B1    C2
        D3
     E4    F5
        G6
     */
    QList<QLineF> segments;
    segments<<QLineF(0,0,30,0)
	    <<QLineF(0,0,0,30)
	    <<QLineF(30,0,30,30)
	    <<QLineF(0,30,30,30)
	    <<QLineF(0,30,0,60)
	    <<QLineF(30,30,30,60)
	    <<QLineF(0,60,30,60);
    for(int i=0;i<7;i++){
	if(values[i]){
	    painter->setPen(onPen);
	}else{
	    painter->setPen(offPen);
	}
	
	painter->drawLine(segments[i].translated(boundingRect().topLeft()+QPointF(10,(boundingRect().height()-60)/2)));
    }
}

QRectF HexOutput::boundingRect() const{
    return QRectF(-25,-25,width,height);
}

void HexOutput::recalculate(){
    for (int i=0;i<7;i++){
	values[i]=myInputs[i]->value();
    }
    update();
}

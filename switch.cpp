#include "switch.h"
#include <QRadialGradient>

Switch::Switch(QObject *parent) :
    Element(parent)
{
    value=0;
    height=50;
    width=50;
    addOutput(new Connection);
    setData(ElementName,"Switch");
    setMinMaxInputsOutputs(0,0,1,1);
    myType="switch";
    tr("Switch");
}

void Switch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Element::paint(painter,option,widget);
    painter->drawRect(boundingRect().adjusted(15,10,-15,-10));
    QBrush upperBrush, lowerBrush;
    QRectF upper, lower;
    upper=boundingRect().adjusted(15,10,-15,-10);
    lower=upper;
    upper.setHeight(upper.height()/2);
    lower.setTop(lower.top()+upper.height());
    lower.setHeight(upper.height());    
    QLinearGradient darkGradient;
    darkGradient.setColorAt(0, QColor(200,200,200));
    darkGradient.setColorAt(1, QColor(180,180,180));
    if(!value){
	darkGradient.setStart(lower.bottomLeft());
	darkGradient.setFinalStop(lower.topLeft());
	upperBrush=QBrush("white");
	lowerBrush=QBrush(darkGradient);
    }else{
	darkGradient.setStart(upper.topLeft());
	darkGradient.setFinalStop(upper.bottomLeft());
	upperBrush=QBrush(darkGradient);
	lowerBrush=QBrush("white");
    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(upperBrush);
    painter->drawRect(upper);
    painter->setBrush(lowerBrush);
    painter->drawRect(lower);
    painter->setPen(QColor(240,240,240));
    if(value){
	painter->drawLine(upper.topLeft()+QPointF(1,1),upper.topRight()+QPointF(-1,1));
    } else {
	painter->drawLine(lower.bottomLeft()+QPointF(1,-1),lower.bottomRight()+QPointF(-1,-1));
    }
//    QRadialGradient gradient;
//    gradient.setRadius(boundingRect().height());
//    gradient.setColorAt(1,QColor(30,30,30));
//    gradient.setColorAt(0,QColor(60,60,60));
//    painter->setBrush(gradient);
//    if(isSelected()){
//	painter->setPen(getSelectionPen());
//    } else{
//	painter->setPen(Qt::NoPen);
//    }
//    painter->drawRoundedRect(boundingRect().adjusted(2,2,-3,-3),10,10);
    
//    //Draw Value
    
//    QColor val;
//    if(value){
//	val=Scene::highValueColor;
//    } else {
//	val=Scene::highValueColor.darker(200);
//    }
//    painter->setBrush(val);
//    painter->setPen(Qt::NoPen);
//    int r=boundingRect().height()/3;
//    painter->drawEllipse(QPoint(0,0),r,r);
}

void Switch::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mousePressEvent(event);
    mouseDownPos=event->scenePos();
}

void Switch::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mouseReleaseEvent(event);
    if(event->scenePos()==mouseDownPos){
	value=!value;
	foreach(Connection* c, myOutputs){
	    if(value)c->setValue(High);
	    if(!value)c->setValue(Low);
	}

	update();
    }
}

QRectF Switch::boundingRect() const
{
    return QRectF(-25,-25,width,height);
}

void Switch::setPrivateXml(QCoreXmlStreamWriter *xml){
    xml->writeAttribute("value",value?"true":"false");
}

void Switch::readPrivateXml(QCoreXmlStreamReader *xml){
    value=(xml->attributes().value("value").toString()=="true"?1:0);
    foreach(Connection* c, myOutputs){
	c->setValue(value);
    }
}

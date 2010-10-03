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
}

void Switch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QRadialGradient gradient;
    gradient.setRadius(boundingRect().height());
    gradient.setColorAt(1,QColor(30,30,30));
    gradient.setColorAt(0,QColor(60,60,60));
    painter->setBrush(gradient);
    if(isSelected()){
	painter->setPen(getSelectionPen());
    } else{
	painter->setPen(Qt::NoPen);
    }
    painter->drawRoundedRect(boundingRect().adjusted(2,2,-3,-3),10,10);
    
    //Draw Value
    
    QColor val;
    if(value){
	val=Scene::highValueColor;
    } else {
	val=Scene::highValueColor.darker(200);
    }
    painter->setBrush(val);
    painter->setPen(Qt::NoPen);
    int r=boundingRect().height()/3;
    painter->drawEllipse(QPoint(0,0),r,r);
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

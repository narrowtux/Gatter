#include "src/elements/button.h"

#include "src/elements/gatter.h"
Button::Button(QGraphicsObject *parent) :
		Switch(parent)
{
    setData(ElementName,"Button");
    myType="button";
    connect(&delay,SIGNAL(timeout()),this,SLOT(updateValue()));
    delay.setSingleShot(true);
    tr("Button");
    pressed=0;
}

void Button::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mouseMoveEvent(event);
    if((startPos-event->pos()).manhattanLength()>QApplication::startDragDistance()){
		if(event->button()==Qt::LeftButton){
			foreach(Connection* c, myOutputs){
				c->setValue(Low);
			}
			myValue=0;
			update();
		}
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    startPos=event->pos();
    if(event->button()==Qt::LeftButton&&boundingRect().adjusted(15,15,-15,-15).contains(event->pos())){
		foreach(Connection* c, myOutputs){
			c->setValue(High);
		}
		myValue=1;
		update();
    }
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
		delay.start((Gatter::delayMS+100)*2);
    }
	Element::mouseReleaseEvent(event);
}


void Button::updateValue(){
    foreach(Connection* c, myOutputs){
		c->setValue(Low);
    }
    myValue=0;
    update();
}

void Button::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter,option,widget);
    QRectF rect=boundingRect().adjusted(15,10,-15,-10);
    painter->drawRect(rect);
    QLinearGradient gradient;
    gradient.setColorAt(!myValue,QColor(220-myValue*50,220-myValue*50,220-myValue*50));
    gradient.setColorAt(myValue,QColor(180-myValue*50,180-myValue*50,180-myValue*50));
    gradient.setStart(rect.topLeft());
    gradient.setFinalStop(rect.bottomLeft());
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(gradient));
    painter->drawRect(rect);
}

void Button::keyTriggered(){
	setInput(1);
	delay.start((Gatter::delayMS+100)*2);
}

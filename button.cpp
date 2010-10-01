#include "button.h"

Button::Button(QObject *parent) :
    Switch(parent)
{
    setData(ElementName,"Button");
    myType="button";
}

void Button::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mouseMoveEvent(event);
    if((startPos-event->pos()).manhattanLength()>QApplication::startDragDistance()){
	if(event->button()==Qt::LeftButton){
	    foreach(Connection* c, myOutputs){
		c->setValue(Low);
	    }
	    value=0;
	    update();
	}
    }
}

void Button::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mousePressEvent(event);
    startPos=event->pos();
    if(event->button()==Qt::LeftButton){
	foreach(Connection* c, myOutputs){
	    c->setValue(High);
	}
	value=1;
	update();
    }
}

void Button::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mouseReleaseEvent(event);
    if(event->button()==Qt::LeftButton){
	foreach(Connection* c, myOutputs){
	    c->setValue(Low);
	}
	value=0;
	update();
    }
}

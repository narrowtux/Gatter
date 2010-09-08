#include "gelement.h"
#include "oelement.h"
GElement::GElement(QObject *parent, QGraphicsItem *gparent) :
    QObject(parent), QGraphicsItem(gparent)
{
}

void GElement::setObject(OElement *object){
	myObject=object;
}

OElement* GElement::object(){
	return myObject;
}

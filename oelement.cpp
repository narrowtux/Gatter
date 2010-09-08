#include "oelement.h"
#include "gelement.h"
OElement::OElement(QObject *parent) :
    QObject(parent)
{
}

void OElement::setGraphic(GElement *graphic){
	myGraphic=graphic;
}

GElement* OElement::graphic(){
	return myGraphic;
}

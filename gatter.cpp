#include "gatter.h"

Gatter::Gatter(QObject *parent) :
    Element(parent)
{
    height=50;
    width=50;
    myType=AND;
    addInput(new Connection);
    addInput(new Connection);
    addOutput(new Connection);
    setData(ElementName,"Gatter");
    beforeUndefined=true;
    beforeValue=false;
}

void Gatter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen("black");
    painter->setBrush(QColor("white"));
    painter->drawRect(boundingRect().adjusted(2,2,-3,-3));
    QString text;
    switch(myType){
    case AND:
	text="&";
	break;
    case OR:
	text=">0";
	break;
    case NOT:
	text="1";
	break;
    case XOR:
	text="=1";
	break;
    }
    QTextOption o;
    o.setAlignment(Qt::AlignCenter);
    painter->drawText(boundingRect().adjusted(5,5,-5,-5),text,o);
    if(isSelected()){
	painter->setPen(getSelectionPen());
	painter->setBrush(Qt::NoBrush);
	painter->drawRect(boundingRect().adjusted(1,1,-1,-1));
    }
}

QSize Gatter::sizeHint() const{
    return QSize(50,50);
}

void Gatter::setType(Type t){
    myType=t;
    switch(t){
    case AND:
	setData(ElementName,"And");
	break;
    case NOT:
	removeInput(myInputs[0]);
	myOutputs[0]->setNegated(1);
	setData(ElementName,"Not");
	break;
    case OR:
	setData(ElementName,"Or");
	break;
    case XOR:
	setData(ElementName,"XOr");
	break;
    case MULTIPLEXER:
	setData(ElementName,"Multiplexer");
	width=20;
	removeInput(myInputs[0]);
	for(int i=0;i<8;i++){
	    addOutput(new Connection);
	}
	break;
    }
    update(boundingRect());
}

Gatter::Type Gatter::type(){
    return myType;
}

QRectF Gatter::boundingRect() const{
    return QRectF(-25,-25,width,height);
}

void Gatter::recalculate(){
    bool val;
    switch(myType){
    case AND:
	val=1;
	foreach(Connection*c, myInputs){
	    if(c->value()&&val){
		val=1;
	    } else {
		val=0;
	    }
	}
	break;
    case OR:
	val=0;
	foreach(Connection*c, myInputs){
	    val=c->value()|val;
	}
	break;
    case NOT:
	val=myInputs[0]->value();
	break;
    case XOR:
	val=0;
	int co;
	foreach(Connection*c, myInputs){
	    if(c->value()==High){
		co++;
	    }
	    if(co%2!=0){
		val=1;
	    } else {
		val=0;
	    }
	}
	break;
    case MULTIPLEXER:
	val=myInputs[0]->value();
	break;
    }
    if(beforeUndefined||beforeValue!=val){
	foreach(Connection*c, myOutputs){
	    if(val)c->setValue(High);
	    else c->setValue(Low);
	}
	beforeUndefined=false;
	beforeValue=val;
    }
}

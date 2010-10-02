#include "gatter.h"

Gatter::Gatter(QObject *parent) :
    Element(parent)
{
    height=50;
    width=50;
    setMinMaxInputsOutputs(2,-1,1,1);
    setType(AND);
    setData(ElementName,"Gatter");
    beforeUndefined=true;
    beforeValue=false;
    delay=new QTimer;
    connect(delay,SIGNAL(timeout()),this,SLOT(sendChanges()));
    delay->setSingleShot(true);
    myType="gatter";
}

int Gatter::delayMS=0;

void Gatter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->setPen("black");
    painter->setBrush(QColor("white"));
    painter->drawRect(boundingRect().adjusted(2,2,-2,-2));
    QString text;
    switch(myGatterType){
    case AND:
	text="&";
	break;
    case OR:
	text=QString::fromUtf8("≥1");
	break;
    case NOT:
	text="1";
	break;
    case XOR:
	text="=1";
	break;
    case DUPLICATOR:
	text="";
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
    myGatterType=t;
    switch(t){
    case AND:
	setData(ElementName,"And");
	setMinMaxInputsOutputs(2,-1,1,1);
	break;
    case NOT:
	setMinMaxInputsOutputs(1,1,1,1);
	myOutputs[0]->setNegated(1);
	setData(ElementName,"Not");
	break;
    case OR:
	setMinMaxInputsOutputs(2,-1,1,1);
	setData(ElementName,"Or");
	break;
    case XOR:
	setMinMaxInputsOutputs(2,-1,1,1);
	setData(ElementName,"XOr");
	break;
    case DUPLICATOR:
	setData(ElementName,"Duplicator");
	width=30;
	setMinMaxInputsOutputs(1,1,1,-1);
	setOutputs(4);
	break;
    }
    update(boundingRect());
}

Gatter::Type Gatter::type(){
    return myGatterType;
}

QRectF Gatter::boundingRect() const{
    return QRectF(-25,-25,width,height);
}

void Gatter::recalculate(){
    bool val;
    switch(myGatterType){
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
	int co;
	foreach(Connection*c, myInputs){
	    if(c->value()==High){
		co++;
	    }
	}
	if(co%2!=0){
	    val=1;
	} else {
	    val=0;
	}
	break;
    case DUPLICATOR:
	val=myInputs[0]->value();
	break;
    }
    if(beforeUndefined||beforeValue!=val){
	beforeUndefined=false;
	beforeValue=val;
	delay->start(delayMS);
    }
}

void Gatter::sendChanges(){
    foreach(Connection*c, myOutputs){
	c->setValue(beforeValue);
    }
}

void Gatter::setPrivateXml(QCoreXmlStreamWriter *xml)
{
    xml->writeAttribute("gatterType",QString().setNum(myGatterType));
}

void Gatter::readPrivateXml(QCoreXmlStreamReader *xml)
{
    QXmlStreamAttributes attr=xml->attributes();
    Type t=static_cast<Type>(attr.value("gatterType").toString().toInt());
    setType(t);
}


#include "src/elements/gatter.h"

Gatter::Gatter(QGraphicsObject *parent) :
		Element(parent)
{
    height=50;
    width=50;
    minHeight=50;
    setMinMaxInputsOutputs(2,-1,1,1);
    setType(AND);
    setData(ElementName,"Gatter");
    beforeUndefined=true;
    beforeValue=false;
    delay=new QTimer;
    connect(delay,SIGNAL(timeout()),this,SLOT(sendChanges()));
    connect(this,SIGNAL(changed()),this,SLOT(sendChanges()),Qt::QueuedConnection);
    delay->setSingleShot(true);
    myType="gatter";
    tr("Gatter");
}

int Gatter::delayMS=0;

void Gatter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option)
    Q_UNUSED(widget)
    Element::paint(painter,option,widget);
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
    case ICON:
		text="Gatter";
		break;
    }
    QTextOption o;
	QFont f=painter->font();
	f.setPixelSize(12);
	painter->setFont(f);
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
    case ICON:
		setData(ElementName,"Icon");
		setMinMaxInputsOutputs(2,2,1,1);
		setScale(10);
		break;
    }
    update(boundingRect());
	recalculate();
}

Gatter::Type Gatter::type(){
    return myGatterType;
}

QRectF Gatter::boundingRect() const{
    return QRectF(-25,-25,width,height);
}

void Gatter::recalculate(){
    if(Scene::debugMethods)qDebug()<<(void*)this<<","<<count("recalcs")<<", Gatter::recalculate()";
    bool val;
    int myDelay=0;
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
		myDelay=10;
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
	case ICON:
		val=0;
		break;
    }
    if(val!=beforeValue||beforeUndefined){
		emit(changed());
    }
    beforeUndefined=false;
    beforeValue=val;
	//    delay->start(delayMS+myDelay);
}

void Gatter::sendChanges(){
    if(Scene::debugMethods)qDebug()<<(void*)this<<","<<count("sendChanges")<<", Gatter::sendChanges()";
    foreach(Connection*c, myOutputs){
		c->setValue(beforeValue);
    }
}

void Gatter::setPrivateXml(QXmlStreamWriter *xml)
{
    xml->writeAttribute("gatterType",QString().setNum(myGatterType));
}

void Gatter::readPrivateXml(QXmlStreamReader *xml)
{
    QXmlStreamAttributes attr=xml->attributes();
    Type t=static_cast<Type>(attr.value("gatterType").toString().toInt());
    setType(t);
}


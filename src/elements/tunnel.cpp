#include "tunnel.h"
#include "src/defines.h"
#include "src/widgets/enumcombobox.h"
#include "src/scene/scene.h"
Tunnel::Tunnel(QGraphicsObject *parent) :
    Element(parent)
{
	myOther=0;
	myType="tunnel";
	myOppositeFinder=new OppositeFinder(this, TunnelID, this);
	myOppositeFinder->setParentItem(this);
	connect(myOppositeFinder, SIGNAL(foundOther(Element*)), this ,SLOT(onOtherFound(Element*)));
	height=50;
	width=50;
	minHeight=30;
	setEntranceType(Input);
	myOppositeFinder->setPos(boundingRect().left()+10, boundingRect().top()+10);
	resizing = false;
}

QRectF Tunnel::boundingRect() const
{
	return QRectF(-width/2, -height/2, width, height);
}

void Tunnel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Element::paint(painter, option, widget);
}

void Tunnel::recalculate()
{
	if(myEntrance==Output){
		//myOutputs[0]->setValue(myValue);
		int i = 0;
		foreach(bool v, myValues){
			myOutputs[i]->setValue(v);
			i++;
		}
	} else {
		myValue=myInputs[0]->value();
		myValues.resize(0);
		foreach(Connection *c, myInputs){
			myValues.append(c->value());
		}

		if(myOppositeFinder->otherElement()!=0){
			myOther=static_cast<Tunnel*>(myOppositeFinder->otherElement());
			myOther->myValue=myValue;
			myOther->myValues = myValues;
			myOther->recalculate();
		}
	}
}

void Tunnel::setEntranceType(ConnectionType t)
{
	myEntrance=t;
	if(t==Input){
		setMinMaxInputsOutputs(1,8,0,0);
	} else {
		setMinMaxInputsOutputs(0,0,1,8);
	}
	myOppositeFinder->setEntrance(t);
}

bool Tunnel::createFormBefore()
{
	EnumComboBox *combo=new EnumComboBox;
	QLabel* label=new QLabel(tr("Entrance Type"));
	layout->addRow(label, combo);
	additionalWidgets<<label<<combo;
	QMap<int, QString> items;
	items.insert(Input, tr("Input"));
	items.insert(Output, tr("Output"));
	combo->addItems(items);
	combo->setCurrentIndex(myEntrance);
	connect(combo, SIGNAL(currentIndexChanged(int)), this, SLOT(setEntranceTypeInt(int)));
    return true;
}

void Tunnel::setEntranceTypeInt(int type)
{
	setEntranceType((ConnectionType)type);
}

ConnectionType Tunnel::entranceType()
{
	return myEntrance;
}

Tunnel * Tunnel::tunnelExit()
{
	Element *other=myOppositeFinder->otherElement();
	if(other!=0){
		return static_cast<Tunnel*>(other);
	} else {
		return 0;
	}
}

void Tunnel::setPrivateXml(QXmlStreamWriter *xml)
{
	xml->writeAttribute("type", QString().setNum((int)myEntrance));
}

void Tunnel::readPrivateXml(QXmlStreamReader *xml)
{
	setEntranceType((ConnectionType)xml->attributes().value("type").toString().toInt());
}

void Tunnel::connectionsChanged()
{
	if(resizing){
		return;
	}
	if(myOppositeFinder->otherElement()!=0){
		Tunnel*other = static_cast<Tunnel*>(myOppositeFinder->otherElement());
		if(other->myInputs.count()==myOutputs.count()&&other->myOutputs.count()==myInputs.count()){
			return;
		}
		other->resizing = true;
		other->setInputs(myOutputs.count());
		other->setOutputs(myInputs.count());
		other->myValues.resize(qMax(myOutputs.count(), myInputs.count()));
		other->resizing = false;
	}
}

void Tunnel::onOtherFound(Element *element)
{
	Tunnel * other = static_cast<Tunnel*>(element);
	other->resizing = true;
	other->setInputs(myOutputs.count());
	other->setOutputs(myInputs.count());
	other->myValues.resize(qMax(myOutputs.count(), myInputs.count()));
	other->resizing = false;
}

OppositeFinder * Tunnel::oppositeFinder()
{
	return myOppositeFinder;
}



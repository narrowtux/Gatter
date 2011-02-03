#include "shiftregister.h"

ShiftRegister::ShiftRegister(QGraphicsObject *parent) :
    Element(parent)
{
    setData(ElementName,"ShiftRegister");
    myType="shiftregister";
	
    myValue=0;
    height=50;
    width=50;
	setMinMaxInputsOutputs(7,19,4,16);
	beforeClockValue = false;
}

QRectF ShiftRegister::boundingRect() const
{
	return QRectF(0,0,width,height);
}

void ShiftRegister::connectionsChanged()
{
	if(myInputs.count()>=3){
		myInputs.at(0)->setName("D");
		myInputs.at(1)->setName("C");
		myInputs.at(2)->setName("T");
		connect(myInputs.at(1), SIGNAL(changed(bool)), this, SLOT(clock(bool)));
	}
	if(myInputs.count()>=4){
		for (int i = 3; i<myInputs.count(); i++){
			myInputs.at(i)->setName(QString("I%0").arg(i-2));
		}
	}
	for (int i = 0; i<myOutputs.count(); i++){
		myOutputs.at(i)->setName(QString("Q%0").arg(i+1));
	}
	if(myOutputs.count()>=4){
		int beforeSize = myRegister.size();
		myRegister.resize(myOutputs.count());
		bool *data = myRegister.data();
		for(int i = beforeSize; i < myRegister.count(); i++){
			data[i] = false;
		}
	}
}

void ShiftRegister::shift()
{
	myRegister.push_front(myInputs[0]->value());
	myRegister.pop_back();
}

void ShiftRegister::clock(bool value)
{
	if(value&&!beforeClockValue){
		shift();
		recalculate();
	}
	beforeClockValue = value;
}

void ShiftRegister::recalculate()
{
	if(myInputs.at(2)->value()==1){
		//Set Data from Inputs
		for(int i = 3; i<myInputs.count(); i++){
			myRegister.append(myInputs.at(i)->value());
			myRegister.pop_front();
		}
	}
	for(int i = 0; i < myRegister.count(); i++){
		myOutputs[i]->setValue(myRegister[i]);
	}
}

void ShiftRegister::setPrivateXml(QXmlStreamWriter *xml)
{
	QString myRegisterCSV = "";
	foreach(bool value, myRegister){
		myRegisterCSV+=QString("%0;").arg((int)value);
	}
	myRegisterCSV.remove(myRegisterCSV.count()-1,1);
	xml->writeAttribute("values", myRegisterCSV);
}

void ShiftRegister::readPrivateXml(QXmlStreamReader *xml)
{
	if(xml->attributes().hasAttribute("values")){
		QString myRegisterCSV = xml->attributes().value("values").toString();
		QStringList myValues = myRegisterCSV.split(";");
		foreach(QString value, myValues){
			bool bvalue = value.toInt();
			myRegister.append(bvalue);		//Reverse shift here. How great!
			myRegister.pop_front();
		}
	}
	recalculate();
}

void ShiftRegister::relayoutConnections()
{
	Element::relayoutConnections();
}

bool ShiftRegister::createFormBefore()
{
	QLabel *bitCountLabel = new QLabel(tr("Size"));
	QSpinBox *bitCountSpinBox = new QSpinBox;
	bitCountSpinBox->setRange(4,16);
	bitCountSpinBox->setValue(myOutputs.count());
	layout->addRow(bitCountLabel, bitCountSpinBox);
	connect(bitCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bitCountChanged(int)));
	additionalWidgets<<bitCountLabel<<bitCountSpinBox;
	return false;
}

void ShiftRegister::bitCountChanged(int n)
{
	setInputs(n+3);
	setOutputs(n);
}

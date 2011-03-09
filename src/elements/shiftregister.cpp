#include "shiftregister.h"

ShiftRegister::ShiftRegister(QGraphicsObject *parent) :
    Element(parent)
{
    setData(ElementName,"ShiftRegister");
    myType="shiftregister";
	
    myValue=0;
    height=50;
    width=50;
	setMinMaxInputsOutputs(7,19,5,17);
	beforeClockValue = false;
	bitCountChanged(4);
}

QRectF ShiftRegister::boundingRect() const
{
	return QRectF(0,0,width,height);
}

void ShiftRegister::connectionsChanged()
{
	if(myInputs.count()>=3){
		myInputs.at(0)->setName("SE");
		myInputs.at(1)->setName("C");
		myInputs.at(2)->setName("PE");
		connect(myInputs.at(1), SIGNAL(changed(bool)), this, SLOT(clock(bool)));
	}
	if(myInputs.count()>=4){
		for (int i = 3; i<myInputs.count(); i++){
			//myInputs.at(i)->setName(QString("I%0").arg(i-2));
			myInputs.at(i)->setName("");
		}
	}
	for (int i = 0; i<myOutputs.count()-1; i++){
		//myOutputs.at(i)->setName(QString("Q%0").arg(i+1));
		myOutputs.at(i)->setName("");
	}
	if(myOutputs.count()>=4){
		int beforeSize = myRegister.size();
		myRegister.resize(myOutputs.count()-1);
		bool *data = myRegister.data();
		for(int i = beforeSize; i < myRegister.count(); i++){
			data[i] = false;
		}
	}
	if(!myOutputs.isEmpty())
		myOutputs.last()->setName(tr("Q%0").arg(myRegister.count()));
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
	myOutputs.at(myOutputs.count()-1)->setValue(myRegister.at(myRegister.count()-1));
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
	connectionsChanged();
	//Q% on the Top, I% Bottom, everything else Left
	QList<Connection *> left, top, bottom, right;
	if(myInputs.count()-3==myRegister.size()&&myOutputs.count()-1==myRegister.size()){
		for(int i = 0; i < 3; i++)
		{
			left<<myInputs.at(i);
		}
		for(int i = 3; i < myInputs.count(); i++)
		{
			bottom<<myInputs.at(i);
		}
		for(int i = 0; i < myOutputs.count()-1; i++)
		{
			top<<myOutputs.at(i);
		}
		right<<myOutputs.at(myOutputs.count()-1);
		//Calculate optimal height
		qreal minimumHeight = qMax(left.count()*20,right.count()*20);
		if(minimumHeight>height){
			prepareGeometryChange();
			height = minimumHeight;
		} else if(minimumHeight<=height){
			if(minimumHeight>minHeight)
			{
				prepareGeometryChange();
				height = minimumHeight;
			} else {
				prepareGeometryChange();
				height = minHeight;
			}
		}
		
		//Calculate optimal width;
		qreal minimumWidth = top.count()*20;
		if(minimumWidth>height){
			prepareGeometryChange();
			width = minimumWidth;
		} else if(minimumWidth<=width){
			if(minimumWidth>minWidth)
			{
				prepareGeometryChange();
				width = minimumWidth;
			} else {
				prepareGeometryChange();
				width = minWidth;
			}
		}
		
		//Distribute Connections
		qreal leftSide=boundingRect().left();
	    qreal topSide=boundingRect().top();
		qreal bottomSide=boundingRect().bottom();
		qreal rightSide=boundingRect().right();
		//left
		int count = left.count();
		for(int i = 0; i < count; i++)
		{
			left[i]->setPos(leftSide-20,(height/(qreal)(count+1))*(1+i)+topSide);
			left[i]->setRotation(0);
		}
		//top
		count = top.count();
		for(int i = 0; i < count; i++){
			top[i]->setPos((width/(qreal)(count+1))*(1+i)+leftSide, topSide);
			top[i]->setRotation(-90);
		}
		
		//bottom
		count = bottom.count();
		for(int i = 0; i < count; i++){
			bottom[i]->setPos((width/(qreal)(count+1))*(1+i)+leftSide, bottomSide+20);
			bottom[i]->setRotation(-90);
		}
		//right
		count = right.count();
		for(int i = 0; i < count; i++){
			right[i]->setPos(rightSide, (height/(qreal)(count+1))*(1+i)+topSide);
			right[i]->setRotation(0);
		}
	}
}

bool ShiftRegister::createFormBefore()
{
	QLabel *bitCountLabel = new QLabel(tr("Size"));
	QSpinBox *bitCountSpinBox = new QSpinBox;
	bitCountSpinBox->setRange(4,16);
	bitCountSpinBox->setValue(myRegister.size());
	layout->addRow(bitCountLabel, bitCountSpinBox);
	connect(bitCountSpinBox, SIGNAL(valueChanged(int)), this, SLOT(bitCountChanged(int)));
	additionalWidgets<<bitCountLabel<<bitCountSpinBox;
	return false;
}

void ShiftRegister::bitCountChanged(int n)
{
	setInputs(n+3);
	setOutputs(n+1);
	if(!myOutputs.isEmpty())
		myOutputs.last()->setName(tr("Q%0").arg(n));
}

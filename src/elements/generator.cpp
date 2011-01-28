#include "generator.h"
#include <QTableView>
Generator::Generator(QGraphicsObject *parent) :
    Element(parent)
{    
	myBitPattern = new BitPattern;
	myRows = 8;
	myOutputCount = 4;
	myBitPattern->init(myRows, myOutputCount);
	connect(myBitPattern, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(recalculate()));
	setData(ElementName,"Generator");
	myType="generator";
	height = 50;
	width = 50;
	setMinMaxInputsOutputs(1,1,1,16);
	setOutputs(myOutputCount);
	inputTypeSelected(0);
	myCurrentRow = 0;
	myBeforeValue = false;
	myInputType = 0;
}

QRectF Generator::boundingRect() const
{
	return QRectF(0,0,width, height);
}

bool Generator::createFormBefore()
{
	QTableView*view = new QTableView;
	connect(view, SIGNAL(clicked(QModelIndex)), this, SLOT(indexClicked(QModelIndex)));
	view->horizontalHeader()->setDefaultSectionSize(16);
	view->verticalHeader()->setDefaultSectionSize(16);
	view->setModel(myBitPattern);
	additionalWidgets<<view;
	layout->addRow(view);
	QSpinBox *spinBoxRows = new QSpinBox;
	additionalWidgets<<spinBoxRows;
	spinBoxRows->blockSignals(true);
	connect(spinBoxRows, SIGNAL(valueChanged(int)), this, SLOT(rowsChanged(int)));
	spinBoxRows->setRange(1,65536);
	spinBoxRows->setValue(myRows);
	spinBoxRows->blockSignals(false);
	QLabel *labelRows = new QLabel(tr("Rows"));
	layout->addRow(labelRows, spinBoxRows);
	additionalWidgets<<labelRows;
	QSpinBox *spinBoxColumns = new QSpinBox;
	spinBoxColumns->blockSignals(true);
	connect(spinBoxColumns, SIGNAL(valueChanged(int)), this, SLOT(outputCountChanged(int)));
	spinBoxColumns->setRange(1,16);
	spinBoxColumns->setValue(myOutputCount);
	spinBoxColumns->blockSignals(false);
	additionalWidgets<<spinBoxColumns;
	QLabel *labelCols = new QLabel(tr("Outputs"));
	additionalWidgets<<labelCols;
	layout->addRow(labelCols, spinBoxColumns);
	QComboBox *comboInputType = new QComboBox;
	QLabel *labelInput = new QLabel(tr("Input Type"));
	comboInputType->addItem(tr("Clock Input"));
	comboInputType->addItem(tr("Number Input"));
	layout->addRow(labelInput, comboInputType);
	additionalWidgets<<comboInputType<<labelInput;
	connect(comboInputType, SIGNAL(currentIndexChanged(int)), this, SLOT(inputTypeSelected(int)));
	return false; 
}

void Generator::indexClicked(const QModelIndex &index)
{
	bool value = myBitPattern->data(index, Qt::DisplayRole).toBool();
	value = !value;
	myBitPattern->setData(index, value, Qt::DisplayRole);
}

void Generator::rowsChanged(int rows)
{
	if(rows!=myRows){
		myBitPattern->init(rows, myOutputCount);
		if(myInputType){
			myRows = rows;
			inputTypeSelected(1);
		}
	}
	myRows = rows;
}

void Generator::outputCountChanged(int count)
{
	if(count != myOutputCount){
		myBitPattern->init(myRows, count);
		setOutputs(count);
	}
	myOutputCount = count;
}

void Generator::setPrivateXml(QXmlStreamWriter *xml)
{
	xml->writeAttribute("bitpattern",myBitPattern->toString());
	xml->writeAttribute("currentrow", QString("%0").arg(myCurrentRow));
	xml->writeAttribute("inputtype", QString("%0").arg(myInputType));
}

void Generator::readPrivateXml(QXmlStreamReader *xml)
{
	if(xml->attributes().hasAttribute("bitpattern")){
		myBitPattern->initFromString(xml->attributes().value("bitpattern").toString());
		myRows = myBitPattern->rowCount(QModelIndex());
		myOutputCount = myBitPattern->columnCount(QModelIndex());
		setOutputs(myOutputCount);
	}
	if(xml->attributes().hasAttribute("currentrow")){
		myCurrentRow = xml->attributes().value("currentrow").toString().toInt();
	}
	if(xml->attributes().hasAttribute("inputtype")){
		myInputType = xml->attributes().value("inputtype").toString().toInt();
		inputTypeSelected(myInputType);
	}
}

void Generator::clock(bool value)
{
	if(value && value!=myBeforeValue){
		myCurrentRow++;
		if(myCurrentRow>=myRows){
			myCurrentRow=0;
		}
		recalculate();
	}
	myBeforeValue = value;
}

void Generator::recalculate()
{
	if(myInputType==1){
		int exp = 0;
		int number = 0;
		foreach(Connection *c, myInputs){
			number+=pow(2,exp)*c->value();
			exp++;
		}
		myCurrentRow = number;
		if(myCurrentRow>=myRows){
			myCurrentRow = myRows-1;
		}
	}
	for (int i = 0; i<myOutputCount; i++)
	{
		QModelIndex index = myBitPattern->index(myCurrentRow, i);
		bool value = myBitPattern->data(index, Qt::DisplayRole).toInt();
		myOutputs.at(i)->setValue(value);
	}
}

void Generator::inputTypeSelected(int type)
{
	switch(type)
	{
	case 0:
		setMinMaxInputsOutputs(1,1,1,16);
		myInputs[0]->setName("C");
		connect(myInputs[0], SIGNAL(changed(bool)), this, SLOT(clock(bool)));
		break;
	case 1:
		int exp = ceil(log2((qreal)myRows));
		qDebug()<<log2(myRows)<<myRows;
		setMinMaxInputsOutputs(exp,exp,1,16);
		if(myInputs.count()>0)
			disconnect(myInputs[0], SIGNAL(changed(bool)), this, SLOT(clock(bool)));
		recalculate();
		break;
	}
	myInputType = type;
}

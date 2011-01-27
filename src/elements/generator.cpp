#include "generator.h"
#include <QTableView>
Generator::Generator(QGraphicsObject *parent) :
    Element(parent)
{    
	myBitPattern = new BitPattern;
	myRows = 8;
	myOutputCount = 4;
	myBitPattern->init(myRows, myOutputCount);
	setData(ElementName,"Generator");
	myType="generator";
	height = 50;
	width = 50;
	setMinMaxInputsOutputs(1,1,1,16);
	setOutputs(myOutputCount);
	myInputs[0]->setName("C");
	connect(myInputs[0], SIGNAL(changed(bool)), this, SLOT(clock(bool)));
	myCurrentRow = 0;
	myBeforeValue;
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
	if(rows!=myRows)
		myBitPattern->init(rows, myOutputCount);
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
	for (int i = 0; i<myOutputCount; i++)
	{
		QModelIndex index = myBitPattern->index(myCurrentRow, i);
		bool value = myBitPattern->data(index, Qt::DisplayRole).toInt();
		myOutputs.at(i)->setValue(value);
	}
}

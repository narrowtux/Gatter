#include "scene.h"
#include "element.h"
#include "mainwindow.h"
#include "button.h"
#include "clock.h"
#include "gatter.h"
#include "lamp.h"
#include "switch.h"
#include "subscene.h"
#include "delay.h"
#include "flipflop.h"
#include "hexoutput.h"
#include <qxmlstream.h>

bool Scene::debugMethods=false;

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
	rect=0;
	pressed=false;
	connect(this,SIGNAL(changed()),this,SIGNAL(modified()));
	QSettings settings;
	highValueColor=settings.value("highValueColor",QColor("red")).value<QColor>();
	blank=true;
	loads=false;
	myMainWindow=0;
}

Scene::~Scene(){
    foreach(Element* e, elements){
	delete e;
    }
}

bool Scene::isLoading(){
    return loads;
}

QColor Scene::highValueColor=QColor("red");

QRectF Scene::rectFromPoints(QPointF p1, QPointF p2){
	qreal x1, x2, y1, y2;
	x1=p1.x();
	y1=p1.y();
	x2=p2.x();
	y2=p2.y();
	QRectF ret;
	ret.setTop(qMin(y1,y2));
	ret.setLeft(qMin(x1,x2));
	ret.setBottom(qMax(y1,y2));
	ret.setRight(qMax(x1,x2));
	return ret;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mousePressEvent(event);
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseMoveEvent(event);
    lastMousePos=event->scenePos();
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseReleaseEvent(event);
}

void Scene::addElement(Element *e,int uniqueId){
    addItem(e);
    e->setPos(lastMousePos);
    if(myMainWindow!=0)
	e->setFormLayout(myMainWindow->getFormLayout());
    if(uniqueId==-1){
	int maxID=0;
	foreach(int key, elements.keys()){
	    if(key>maxID){
		maxID=key;
	    }
	}
	elements.insert(maxID+1,e);
	e->uniqueId=maxID+1;
    } else {
	elements.insert(uniqueId,e);
	e->uniqueId=uniqueId;
    }
    emit(modified());
    emit(elementAddedOrRemoved());
    blank=false;
}

void Scene::removeElement(Element *e){
    //Warning: this does not delete the removed Element!
    e->deleteForm();
    QGraphicsScene::removeItem(e);
    elements.remove(e->uniqueId);
}

void Scene::removeItem(QGraphicsItem *item){
    if(isElement(item)){
	removeElement(static_cast<Element*>(item));
    } else {
	QGraphicsScene::removeItem(item);
    }
    emit(modified());
    emit(elementAddedOrRemoved());
}

bool Scene::isElement(QGraphicsItem *item){
    return elements.contains(elements.key(static_cast<Element*>(item)));
}

void Scene::setMainWindow(MainWindow *m){
    myMainWindow=m;
    foreach(Element*e, elements){
	e->setFormLayout(m->getFormLayout());
    }
}

void Scene::setScale(qreal scale){
    foreach(Element* e, elements){
	e->setScale(scale);
    }
}

void Scene::load(QString fileName, QXmlStreamReader *xml, bool setAllAttributes)
{
    loads=true;
    clear();
    bool own=false;
    QFile file(fileName);
    if(xml==0){
	if(!file.open(QIODevice::ReadOnly)){
	    return;
	}
	xml=new QXmlStreamReader;
	xml->setDevice(&file);
	own=true;
    }
    while(!(xml->name()=="scene"&&xml->isEndElement())){
	xml->readNext();
	qDebug()<<xml->name()<<xml->isStartElement();
	if(xml->name()=="connections"){
	    while(!(xml->name()=="connections"&&xml->isEndElement())){
		xml->readNext();
		qDebug()<<xml->name()<<xml->isStartElement();
		if(xml->name()=="connect"&&xml->isStartElement()){
		    QXmlStreamAttributes attr=xml->attributes();
		    int inElement, outElement, input, output;
		    inElement=attr.value("inElement").toString().toInt();
		    outElement=attr.value("outElement").toString().toInt();
		    input=attr.value("input").toString().toInt();
		    output=attr.value("output").toString().toInt();
		    connectItems(inElement,outElement,input,output);
		}
	    }
	}
	if(xml->name()=="element"){
	    QXmlStreamAttributes attr=xml->attributes();
	    QString elementType=attr.value("type").toString();
	    Element* element=getElementFromTypeName(elementType);
	    if(element!=0){
		addElement(element,attr.value("id").toString().toInt());
		element->setX(attr.value("x").toString().toDouble());
		element->setY(attr.value("y").toString().toDouble());
		int count=0;
		while(!(xml->name()=="inputs"&&xml->isEndElement()))
		{
		    xml->readNext();
		    qDebug()<<xml->name()<<xml->isStartElement();
		    if(setAllAttributes){
			if(xml->name()=="private"&&xml->isStartElement()){
			    element->readPrivateXml(xml);
			}
		    }
		    if(xml->name()=="connection"&&xml->isStartElement()){
			count++;
			attr=xml->attributes();
			int id=attr.value("id").toString().toInt();
			bool negated=(attr.value("negated").toString()=="true"?1:0);
			QString label=attr.value("name").toString();
			bool value=(attr.value("value").toString()=="true"?1:0);
			Connection*c;
			if(element->myInputs.count()>=id+1){
			    c=element->myInputs.value(id);
			}else{
			    element->addInput(1);
			    c=element->myInputs.last();
			}
			if(setAllAttributes){
			    c->setNegated(negated);
			    c->setValue(value);
			}
			c->setName(label);
		    }
		}
		element->setInputs(count);
		count=0;
		while(!(xml->name()=="outputs"&&xml->isEndElement())){
		    xml->readNext();
		    qDebug()<<xml->name()<<xml->isStartElement();
		    if(xml->name()=="connection"&&xml->isStartElement()){
			attr=xml->attributes();
			int id=attr.value("id").toString().toInt();
			bool negated=(attr.value("negated").toString()=="true"?1:0);
			QString label=attr.value("name").toString();
			bool value=(attr.value("value").toString()=="true"?1:0);
			Connection*c;
			if(element->myOutputs.count()>=id+1){
			    c=element->myOutputs.value(id);
			}else{
			    element->addOutput(1);
			    c=element->myOutputs.last();
			}
			c->setValue(value);
			c->setNegated(negated);
			c->setName(label);
			count++;
		    }
		}
		element->setOutputs(count);
	    }
	}
    }
   if(own)file.close();
   loads=false;
}

void Scene::save(QString fileName, QXmlStreamWriter *xml)
{
    bool own=false;
    QFile file(fileName);
    if(xml==0){
	xml=new QXmlStreamWriter;
	file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate);
	xml->setDevice(&file);
	xml->setAutoFormatting(true);
	xml->writeStartDocument();
	own=true;
    }
    xml->writeStartElement("scene");
    /*Elements*/{
	xml->writeStartElement("elements");
	foreach(Element* e,elements){
	    xml->writeStartElement("element");
	    QXmlStreamAttributes elementAttributes;
	    elementAttributes.append("x",QString().setNum(e->scenePos().x()));
	    elementAttributes.append("y",QString().setNum(e->scenePos().y()));
	    elementAttributes.append("id",QString().setNum(e->uniqueId));
	    elementAttributes.append("type",e->myType);
	    xml->writeAttributes(elementAttributes);
	    xml->writeStartElement("private");
	    e->setPrivateXml(xml);
	    xml->writeEndElement();
	    xml->writeStartElement("inputs");
	    
	    int count=0;
	    foreach(Connection*c,e->myInputs)
	    {
		xml->writeStartElement("connection");
		QXmlStreamAttributes connectionAttributes;
		connectionAttributes.append("id",QString().setNum(count));
		connectionAttributes.append("name",c->name());
		connectionAttributes.append("negated",c->isNegated()?"true":"false");
		connectionAttributes.append("value",c->value()?"true":"false");
		xml->writeAttributes(connectionAttributes);
		xml->writeEndElement();
		count++;
	    }

	    xml->writeEndElement();
	    xml->writeStartElement("outputs");
	    
	    count=0;
	    foreach(Connection*c,e->myOutputs)
	    {
		xml->writeStartElement("connection");
		QXmlStreamAttributes connectionAttributes;
		connectionAttributes.append("id",QString().setNum(count));
		connectionAttributes.append("name",c->name());
		connectionAttributes.append("negated",c->isNegated()?"true":"false");
		connectionAttributes.append("value",c->value()?"true":"false");
		xml->writeAttributes(connectionAttributes);
		xml->writeEndElement();
		count++;
	    }
	    
	    xml->writeEndElement();
	    xml->writeEndElement();
	}
	xml->writeEndElement();
    }
    /*Connections*/{
	xml->writeStartElement("connections");
	foreach(Element*e,elements){
	    foreach(Connection*c,e->myInputs)
	    {
		if(c->isConnected())
		{
		    xml->writeStartElement("connect");
		    QXmlStreamAttributes connectionAttributes;
		    connectionAttributes.append("inElement",QString().setNum(c->element()->uniqueId));
		    connectionAttributes.append("outElement",QString().setNum(c->connectedTo()->element()->uniqueId));
		    int id;
		    id=e->myInputs.indexOf(c);
		    connectionAttributes.append("input",QString().setNum(id));
		    id=c->connectedTo()->element()->myOutputs.indexOf(c->connectedTo());
		    connectionAttributes.append("output",QString().setNum(id));
		    xml->writeAttributes(connectionAttributes);
		    xml->writeEndElement();
		}
	    }
	}
	xml->writeEndElement();
    }
    xml->writeEndElement();
    if(own){
	xml->writeEndDocument();
	file.close();
    }
}

Element* Scene::getElementFromTypeName(QString typeName){
    if(typeName=="gatter")
	return new Gatter;
    if(typeName=="button")
	return new Button;
    if(typeName=="clock")
	return new Clock;
    if(typeName=="lamp")
	return new Lamp;
    if(typeName=="switch")
	return new Switch;
    if(typeName=="subscene")
	return new SubScene;
    if(typeName=="delay")
	return new Delay;
    if(typeName=="flipflop")
	return new FlipFlop;
    if(typeName=="hexoutput")
	return new HexOutput;
    return 0;
}

void Scene::connectItems(int inElement, int outElement, int input, int output)
{
    Element *in, *out;
    if(elements.contains(inElement)&&elements.contains(outElement)){
	in=elements[inElement];
	out=elements[outElement];
    } else {
	return;
    }
    if(in->myInputs.count()>input&&out->myOutputs.count()>output){
	Connection* inputC=in->myInputs[input];
	Connection* outputC=out->myOutputs[output];
	inputC->connectWith(outputC);
    } else {
	return;
    }
    emit(modified());
}

bool Scene::isBlank(){
    return blank;
}

void Scene::clear(){
    foreach(Element* e, elements){
	removeElement(e);
    }
    elements.clear();
    QGraphicsScene::clear();
}

MainWindow* Scene::mainWindow(){
    return myMainWindow;
}

QGraphicsItem* Scene::itemAt(const QPointF &pos) const{
    QGraphicsItem* item=QGraphicsScene::itemAt(pos), *realItem;
    if(item==0){
	return 0;
    }
    if(item->parentItem()->data(ElementRecognition).toString()=="Connection"){
	QPointF itemPos=item->pos();
	item->setPos(itemPos+QPointF(100,100));
	realItem=QGraphicsScene::itemAt(pos);
	item->setPos(itemPos);
	return realItem;
    }
    return QGraphicsScene::itemAt(pos);
}

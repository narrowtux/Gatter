#include "scene.h"
#include "element.h"
#include "mainwindow.h"
#include "button.h"
#include "clock.h"
#include "gatter.h"
#include "lamp.h"
#include "switch.h"
#include "subscene.h"
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
	bool check=true;
	QGraphicsItem* item=itemAt(event->scenePos());
	if(item!=0){
		if(item->flags()|QGraphicsItem::ItemIsMovable) check=false;
		if(item->flags()|QGraphicsItem::ItemIsFocusable) check=false;
		if(item->flags()|QGraphicsItem::ItemIsSelectable) check=false;
	}
	if(event->button()==Qt::LeftButton&&check){
		pressed=true;
		startPos=event->scenePos();
	}
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseMoveEvent(event);
	if(pressed){
		if(rect==0){
			rect=addRect(rectFromPoints(startPos,event->scenePos()));
			QColor hi=QApplication::palette().highlight().color();
			hi.setAlpha(128);
			rect->setBrush(hi);
			rect->setPen(Qt::NoPen);
		} else {
			rect->setRect(rectFromPoints(startPos,event->scenePos()));
		}
		QPainterPath selection;
		selection.addRect(rect->rect());
		setSelectionArea(selection);
		foreach(QGraphicsView* view, views()){
			view->ensureVisible(event->scenePos().x(),event->scenePos().y(),0,0);
		}
	}
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsScene::mouseReleaseEvent(event);
	if(pressed){
		QPainterPath selection;
		selection.addRect(rectFromPoints(startPos,event->scenePos()));
		if(rect!=0){
			removeItem(rect);
			delete rect;
		}
		rect=0;
		pressed=false;
	}
}

void Scene::addElement(Element *e,int uniqueId){
    addItem(e);
    e->setPos(QPointF(0.5,0.5));
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
}

void Scene::setScale(qreal scale){
    foreach(Element* e, elements){
	e->setScale(scale);
    }
}

void Scene::load(QString fileName, QCoreXmlStreamReader *xml, bool setAllAttributes)
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

void Scene::save(QString fileName, QCoreXmlStreamWriter *xml)
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
	    QCoreXmlStreamAttributes elementAttributes;
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
		QCoreXmlStreamAttributes connectionAttributes;
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
		QCoreXmlStreamAttributes connectionAttributes;
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
		    QCoreXmlStreamAttributes connectionAttributes;
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
    return 0;
}

void Scene::connectItems(int inElement, int outElement, int input, int output)
{
    Element* in=elements[inElement];
    Element* out=elements[outElement];
    Connection* inputC=in->myInputs[input];
    Connection* outputC=out->myOutputs[output];
    inputC->connectWith(outputC);
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

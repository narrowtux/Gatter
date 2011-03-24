
#include "src/scene/scene.h"
#include "src/elements/element.h"

#include "src/widgets/mainwindow.h"
#include "src/elements/button.h"
#include "src/elements/clock.h"

#include "src/elements/gatter.h"
#include "src/elements/lamp.h"
#include "src/elements/switch.h"
#include "src/elements/subscene.h"
#include "src/elements/delay.h"
#include "src/elements/flipflop.h"
#include "src/elements/sevensegmentoutput.h"
#include "src/elements/distributor.h"
#include "src/elements/tunnel.h"
#include "src/elements/shiftregister.h"
#include "src/elements/generator.h"
#include <qxmlstream.h>
#include <QGestureRecognizer>
#include <QMimeData>

bool Scene::debugMethods=false;

/*!
  Constructs the Scene as a child of the given \a parent.
  */

Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
    pressed=false;
    QSettings settings;
    highValueColor=settings.value("highValueColor",QColor("red")).value<QColor>();
    blank=true;
    loads=false;
    myMainWindow=0;
    wantsToDrag=false;
	myHighlighter=new Highlighter;
	myHighlighter->setOpacity(0.0);
	addItem(myHighlighter);
	myHighlighter->setBoundingRect(QRectF(-100,-100,200,200));
	highlight(0);
	subscene=0;
}

/*!
  Desctructs the Scene and all of its containing Elements
  */

Scene::~Scene(){
    foreach(Element* e, myElements){
		delete e;
    }
}

/*!
  \return if the Scene is currently loading
  */
bool Scene::isLoading(){
    return loads;
}

QColor Scene::highValueColor=QColor("red");

/*!
  \return a rect whose boundaries are limited by \a p1 and \a p2.
  */
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
    QPointF mousePos(event->buttonDownScenePos(Qt::LeftButton).x(),
                     event->buttonDownScenePos(Qt::LeftButton).y());
    movingItem=itemAt(mousePos);
    if(movingItem!=0&&isElement(movingItem)){
		startPos=movingItem->scenePos();
    }
	Qt::KeyboardModifier dragMod;
#ifdef QT_ARCH_MACOSX
	dragMod=Qt::AltModifier;
#else
	dragMod=Qt::ControlModifier;
#endif
    if(event->modifiers()&dragMod){
		wantsToDrag=true;
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event){
	Qt::KeyboardModifier dragMod;
#ifdef QT_ARCH_MACOSX
	dragMod=Qt::AltModifier;
#else
	dragMod=Qt::ControlModifier;
#endif
    if(event->modifiers()&dragMod){
		wantsToDrag=true;
    }
    lastMousePos=event->scenePos();
    if(wantsToDrag&&(startPos-event->scenePos()).manhattanLength()>=QApplication::startDragDistance()){
		if(selectedItems().count()>0){
			QList<Element*> selElements;
			foreach(QGraphicsItem* i, selectedItems()){
				if(isElement(i)){
					selElements<<(Element*)i;
				}
			}
			QString xml=copy(selElements);
			QRectF rect=selElements.at(0)->boundingRect();
			QPixmap dragPixmap(rect.width(),rect.height());
			dragPixmap.fill(QColor(0,0,0,0));
			QPainter painter(&dragPixmap);
			painter.setRenderHint(QPainter::Antialiasing);
			render(&painter,QRectF(), selElements[0]->sceneBoundingRect());
			QDrag*drag=new QDrag(views().at(0));
			drag->setPixmap(dragPixmap);
			drag->setHotSpot(selElements[0]->mapFromScene(event->scenePos()).toPoint());
			QMimeData* data=new QMimeData;
			data->setData("text/gatterxml",xml.toLocal8Bit());
			drag->setMimeData(data);
			drag->exec(Qt::CopyAction,Qt::CopyAction);
		}
		wantsToDrag=false;
    } else {
		QGraphicsScene::mouseMoveEvent(event);
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    wantsToDrag=false;
    QGraphicsItem*item=itemAt(event->scenePos());
    if(movingItem!=0&&movingItem==item&&item!=0&&item->flags()&QGraphicsItem::ItemIsMovable){
		QPointF distance=movingItem->pos()-startPos;
		QList<Element*> els;
		QList<QPointF> startPoss;
		foreach(QGraphicsItem*i, selectedItems()){
			if(isElement(i)){
				els<<(Element*)i;
				startPoss<<i->pos()-distance;
			}
		}
		if(els.count()!=0&&distance.manhattanLength()>=QApplication::startDragDistance())
			emit(elementMoved(els,startPoss));
    }
    QGraphicsScene::mouseReleaseEvent(event);
}


/*!
  Adds a new Element \a e with the \a uniqueId to the Scene.
  */

void Scene::addElement(Element *e,int uniqueId){
    addItem(e);
    e->setPos(lastMousePos);
    if(myMainWindow!=0)
		e->setFormLayout(myMainWindow->getFormLayout());
    if(uniqueId==-1){
		int maxID=0;
		foreach(int key, myElements.keys()){
			if(key>maxID){
				maxID=key;
			}
		}
		myElements.insert(maxID+1,e);
		e->uniqueId=maxID+1;
    } else {
		myElements.insert(uniqueId,e);
		e->uniqueId=uniqueId;
    }
    emit(modified());
    emit(elementAddedOrRemoved());
    blank=false;
	connect(e,SIGNAL(moved()),this,SIGNAL(modified()));
}

/*!
  removes the Element \a e from the scene.
  
  \warning This method does not delete the Element!
  */

void Scene::removeElement(Element *e){
    //Warning: this does not delete the removed Element!
    e->deleteForm();
    QGraphicsScene::removeItem(e);
    myElements.remove(e->uniqueId);
    e->releaseConnections();
}

/*!
  removes the QGraphicsItem \a item from the scene.
  */

void Scene::removeItem(QGraphicsItem *item){
    if(isElement(item)){
		removeElement(static_cast<Element*>(item));
    } else {
		QGraphicsScene::removeItem(item);
    }
    emit(modified());
    emit(elementAddedOrRemoved());
}

/*!
  \return true if \a item is an Element.
  */
bool Scene::isElement(QGraphicsItem *item){
    return myElements.contains(myElements.key(static_cast<Element*>(item)));
}

/*!
  sets the MainWindow to \a m
  */

void Scene::setMainWindow(MainWindow *m){
    myMainWindow=m;
    foreach(Element*e, myElements){
		e->setFormLayout(m->getFormLayout());
    }
}

/*!
  sets the scale to \a scale. Useful for zooming.
  */
void Scene::setScale(qreal scale){
    foreach(Element* e, myElements){
		e->setScale(scale);
    }
}

/*!
  Loads the scene.
  
  If \a fileName is given, it will try to load from that file.
  
  If \a xml is not null, it will load from that QXmlStreamReader
  
  If \a setAllAttributes is false, it won't set the private and negation attributes to the loaded Elements
  
  If \a paste is true, it will paste the loaded Elements to the point \a pasteTo
  
  If \a paste is false, it will clear() the current scene
  */
void Scene::load(QString fileName, QXmlStreamReader *xml, bool setAllAttributes, bool paste, QPointF pasteTo)
{
    if(pasteTo.isNull()){
		pasteTo=lastMousePos;
    }
    loads=true;
    if(!paste)
		clear();
    else
		clearSelection();
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
    QMap<int, int> elementIdMapping;
    QList<Element*> pastedElements;
    while(!(xml->name()=="scene"&&xml->isEndElement())&&!xml->hasError()){
		xml->readNext();
		//qDebug()<<xml->name()<<xml->isStartElement();
		if(xml->name()=="element"){
			QXmlStreamAttributes attr=xml->attributes();
			QString elementType=attr.value("type").toString();
			Element* element=getElementFromTypeName(elementType);
			if(element!=0){
				if(!paste){
					addElement(element, attr.value("id").toString().toInt());
				} else {
					addElement(element, -1);
					elementIdMapping.insert(attr.value("id").toString().toInt(),element->uniqueId);
					pastedElements<<element;
				}
				element->setX(attr.value("x").toString().toDouble());
				element->setY(attr.value("y").toString().toDouble());
				QString colorString=xml->attributes().value("elementColor").toString();
				QRegExp exp("rgb\\(([0-9]{1,3}),([0-9]{1,3}),([0-9]{1,3})\\)");
				exp.indexIn(colorString);
				QColor elementColor;
				elementColor.setRed(exp.cap(1).toInt());
				//qDebug()<<exp.capturedTexts();
				elementColor.setGreen(exp.cap(2).toInt());
				elementColor.setBlue(exp.cap(3).toInt());
				element->setElementColor(elementColor);
				element->setRotation(xml->attributes().value("rotation").toString().toDouble());
				int count=0;
				while(!(xml->name()=="inputs"&&xml->isEndElement())&&!xml->hasError())
				{
					xml->readNext();
					//qDebug()<<xml->name()<<xml->isStartElement();
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
				while(!(xml->name()=="outputs"&&xml->isEndElement())&&!xml->hasError()){
					xml->readNext();
					//qDebug()<<xml->name()<<xml->isStartElement();
					if(xml->name()=="connection"&&xml->isStartElement()){
						attr=xml->attributes();
						int id=attr.value("id").toString().toInt();
						bool negated=attr.value("negated").toString()=="true"?1:0;
						QString label=attr.value("name").toString();
						bool value=attr.value("value").toString()=="true"?1:0;
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
				element->loadEvent();
			}
		}
		if(xml->name()=="connections"){
			while(!(xml->name()=="connections"&&xml->isEndElement())&&!xml->hasError()){
				xml->readNext();
				//qDebug()<<xml->name()<<xml->isStartElement();
				if(xml->name()=="connect"&&xml->isStartElement()){
					QXmlStreamAttributes attr=xml->attributes();
					int inElement, outElement, input, output;
					inElement=attr.value("inElement").toString().toInt();
					outElement=attr.value("outElement").toString().toInt();
					if(paste){
						inElement=elementIdMapping.value(inElement);
						outElement=elementIdMapping.value(outElement);
					}
					input=attr.value("input").toString().toInt();
					output=attr.value("output").toString().toInt();
					connectItems(inElement,outElement,input,output);
				}
			}
		}
    }
    bool errors=xml->hasError();
    if(xml->hasError()){
		qDebug()<<"XML load error:"<<xml->error()<<xml->errorString();
    }
    if(!errors){
		QVector<QPointF> points;
		foreach(Element* e, pastedElements){
			e->setSelected(true);
			points.append(e->scenePos());
		}
		QPolygonF p(points);
		QRectF rect=p.boundingRect();
		QPointF before=rect.topLeft();
		p.translate(pasteTo-before);
		points=p.toList().toVector();
		for(int i=0;i<points.count();i++){
			pastedElements.at(i)->setPos(points.at(i));
		}
    }
    if(own)file.close();
    loads=false;
}

/*!
  Saves the current scene.
  
  If \a fileName is given, it will save the scene to this file.
  
  If \a xml is not null, it will write to the QXmlStreamWriter
  
  If \a selectionElements is given, it will just save these Elements and the connections between them.
  */

void Scene::save(QString fileName, QXmlStreamWriter *xml, QList<Element *> selectionElements)
{
    bool own=false;
    if(selectionElements.isEmpty()){
		selectionElements=myElements.values();
    }
    QFile file(fileName);
    if(xml==0){
		xml=new QXmlStreamWriter;
		if(!file.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate)){
			qDebug()<<"Unable to open file for writing: "<<file.fileName();
			return;
		}
		xml->setDevice(&file);
		xml->setAutoFormatting(true);
		xml->writeStartDocument();
		own=true;
    }
    xml->writeStartElement("scene");
    /*Elements*/{
		xml->writeStartElement("elements");
		foreach(Element* e,selectionElements){
			xml->writeStartElement("element");
			QXmlStreamAttributes elementAttributes;
			elementAttributes.append("x",QString().setNum(e->scenePos().x()));
			elementAttributes.append("y",QString().setNum(e->scenePos().y()));
			elementAttributes.append("id",QString().setNum(e->uniqueId));
			elementAttributes.append("type",e->myType);
			elementAttributes.append("elementColor",QString("rgb(%0,%1,%2)").arg(e->elementColor().red()).arg(e->elementColor().green()).arg(e->elementColor().blue()));
			elementAttributes.append("rotation",QString().setNum(e->data(ElementRotation).toReal()));
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
		foreach(Element*e,selectionElements){
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


/*!
  \returns a new Element with the type of the given \a typeName.
  */
Element* Scene::getElementFromTypeName(QString typeName){
    if(typeName == "gatter")
		return new Gatter;
    if(typeName == "button")
		return new Button;
    if(typeName == "clock")
		return new Clock;
    if(typeName == "lamp")
		return new Lamp;
    if(typeName == "switch")
		return new Switch;
    if(typeName == "subscene"){
		SubScene* sub = new SubScene;
		sub->setParentSubScene(subscene);
		return sub;
	}
    if(typeName == "delay")
		return new Delay;
    if(typeName == "flipflop")
		return new FlipFlop;
    if(typeName == "hexoutput")
		return new SevenSegmentOutput;
	if(typeName == "distributor")
		return new Distributor;
	if(typeName == "tunnel")
		return new Tunnel;
	if(typeName == "shiftregister")
		return new ShiftRegister;
	if(typeName == "generator")
		return new Generator;
    return 0;
}

/*!
  connects the elements \a inElement and \a outElements. It will use \a input and \a output as the Connections 
  */

void Scene::connectItems(int inElement, int outElement, int input, int output)
{
    Element *in, *out;
    if(myElements.contains(inElement)&&myElements.contains(outElement)){
		in=myElements[inElement];
		out=myElements[outElement];
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


/*!
  \returns if the scene does not contains any items.
  */
bool Scene::isBlank(){
    return blank;
}

/*!
  Clears the scene.
  */

void Scene::clear(){
    foreach(Element* e, myElements){
		removeElement(e);
    }
    myElements.clear();
    QGraphicsScene::clear();
	myHighlighter=new Highlighter;
	addItem(myHighlighter);
	myHighlighter->setBoundingRect(QRectF(-100,-100,200,200));
	highlight(0);
}
/*!
  \returns the MainWindow
  */
MainWindow* Scene::mainWindow(){
    return myMainWindow;
}

void Scene::dropEvent(QGraphicsSceneDragDropEvent *event){
    const QMimeData*data=event->mimeData();
    if(data->hasFormat("text/gatterxml")){
		event->setDropAction(Qt::CopyAction);
		event->accept();
		paste(data,event->scenePos());
    }else{
		event->ignore();
    }
}

void Scene::dragEnterEvent(QGraphicsSceneDragDropEvent *event){
    QGraphicsScene::dragEnterEvent(event);
    if(event->mimeData()->hasFormat("text/gatterxml")){
		event->setDropAction(Qt::CopyAction);
		event->setProposedAction(Qt::CopyAction);
		event->setPossibleActions(Qt::CopyAction);
		event->accept();
    } else {
		event->ignore();
    }
}

void Scene::dragMoveEvent(QGraphicsSceneDragDropEvent *event){
    if(event->mimeData()->hasFormat("text/gatterxml")){
		event->setDropAction(Qt::CopyAction);
		event->accept();
    } else {
		event->ignore();
    }
    
}

/*!
  Pastes the Elements in \a mimeData to the Point \a pos
  */

void Scene::paste(const QMimeData *mimeData, QPointF pos){
    QXmlStreamReader*xml=new QXmlStreamReader;
    xml->addData(QString(mimeData->data("text/gatterxml")).toLatin1());
    load("",xml,true,true,pos);
    delete xml;
}

/*!
  \returns a QString that contains a saved representation of the given \a elements
  */
QString Scene::copy(QList<Element *> elements){
    QXmlStreamWriter* xml=new QXmlStreamWriter;
    QByteArray array;
    QBuffer buffer(&array);
    buffer.open(QIODevice::WriteOnly);
    xml->setDevice(&buffer);
    xml->writeStartDocument();
    save("",xml,elements);
    xml->writeEndDocument();
    buffer.close();
    //qDebug()<<QString(array);
    return QString(array).toLatin1();
}


/*!
  highlights the given \a element with the Highlighter
  
  If \a element is null, the highlighter will be faded out.
  */
void Scene::highlight(QGraphicsItem *element){
	if(element==0){
		QPropertyAnimation*animation=new QPropertyAnimation(myHighlighter, "opacity");
		animation->setStartValue(myHighlighter->opacity());
		animation->setEndValue(0.0);
		animation->setDuration(200);
		animation->start();
	}else{
		QPropertyAnimation*animation=new QPropertyAnimation(myHighlighter, "opacity");
		animation->setStartValue(myHighlighter->opacity());
		animation->setEndValue(1.0);
		animation->setDuration(200);
		animation->start();
		myHighlighter->setPos(0,0);
		myHighlighter->setBoundingRect(itemsBoundingRect());
		myHighlighter->highlight((QGraphicsItem*)element);
		myHighlighter->setZValue(100);
	}
}

/*!
  highlights the given \a elements with the Highlighter
  
  If \a elements contains 0 items the highlighter will be faded out.
  */
void Scene::highlight(QList<QGraphicsItem *> elements){
	if(elements.count()==0){
		highlight(0);
	}else{
		QPropertyAnimation*animation=new QPropertyAnimation(myHighlighter, "opacity");
		animation->setStartValue(myHighlighter->opacity());
		animation->setEndValue(1.0);
		animation->setDuration(200);
		animation->start();
		myHighlighter->setPos(0,0);
		myHighlighter->setBoundingRect(itemsBoundingRect());
		myHighlighter->highlight(elements);
		myHighlighter->setZValue(100);
	}
}

/*!
  \returns a list with all elements
  */

QList<Element *> Scene::elementList()
{
	return myElements.values();
}
/*!
  Convenience method for calling highlight(0)
  */
void Scene::clearHighlight()
{
	highlight(0);
}

void Scene::setSubscene(SubScene *sub)
{
	subscene = sub;
}


#include "src/elements/switch.h"
#include <QRadialGradient>
#include "src/widgets/qkeysequencewidget/src/qkeysequencewidget.h"

Switch::Switch(QGraphicsObject *parent) :
		Element(parent)
{
    myValue=0;
    height=50;
    width=50;
    addOutput(new Connection);
    setData(ElementName,"Switch");
    setMinMaxInputsOutputs(0,0,1,1);
    myType="switch";
    tr("Switch");
	shortcut=0;
}

Switch::~Switch(){
	delete shortcut;
}

void Switch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Element::paint(painter,option,widget);
    painter->drawRect(boundingRect().adjusted(15,10,-15,-10));
    QBrush upperBrush, lowerBrush;
    QRectF upper, lower;
    upper=boundingRect().adjusted(15,10,-15,-10);
    lower=upper;
    upper.setHeight(upper.height()/2);
    lower.setTop(lower.top()+upper.height());
    lower.setHeight(upper.height());    
    QLinearGradient darkGradient;
    darkGradient.setColorAt(0, QColor(200,200,200));
    darkGradient.setColorAt(1, QColor(180,180,180));
    if(!myValue){
		darkGradient.setStart(lower.bottomLeft());
		darkGradient.setFinalStop(lower.topLeft());
		upperBrush=QBrush(QColor(240,240,240));
		lowerBrush=QBrush(darkGradient);
    }else{
		darkGradient.setStart(upper.topLeft());
		darkGradient.setFinalStop(upper.bottomLeft());
		upperBrush=QBrush(darkGradient);
		lowerBrush=QBrush(QColor(240,240,240));
    }
    painter->setPen(Qt::NoPen);
    painter->setBrush(upperBrush);
    painter->drawRect(upper);
    painter->setBrush(lowerBrush);
    painter->drawRect(lower);
    painter->setPen(QColor(240,240,240));
	QFont f=painter->font();
	f.setPixelSize(10);
	painter->setFont(f);
    if(myValue){
		painter->drawLine(upper.topLeft()+QPointF(1,1),upper.topRight()+QPointF(-1,1));
		painter->setPen(QColor(150,150,150));
		painter->drawLine(lower.bottomLeft()+QPointF(1,-1),lower.bottomRight()+QPointF(-1,-1));
		painter->setPen(QColor("black"));
		painter->drawText(upper, "ON",QTextOption(Qt::AlignCenter|Qt::AlignHCenter));
		painter->setPen(QColor("gray"));
		painter->drawText(lower.adjusted(0,0,-1,0), "OFF",QTextOption(Qt::AlignCenter|Qt::AlignHCenter));
		
    } else {
		painter->drawLine(lower.bottomLeft()+QPointF(1,-1),lower.bottomRight()+QPointF(-1,-1));
		painter->setPen(QColor(150,150,150));
		painter->drawLine(upper.topLeft()+QPointF(1,1),upper.topRight()+QPointF(-1,1));
		painter->setPen(QColor("black"));
		painter->drawText(lower, "OFF", QTextOption(Qt::AlignCenter|Qt::AlignHCenter));
		painter->setPen(QColor("gray"));
		painter->drawText(upper.adjusted(0,1,0,0), "ON",QTextOption(Qt::AlignCenter|Qt::AlignHCenter));
    }
}

void Switch::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mousePressEvent(event);
    if(event->button()==Qt::LeftButton&&boundingRect().adjusted(15,10,-15,-10).contains(event->pos()))
		mouseDownPos=event->scenePos();
}

void Switch::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Element::mouseReleaseEvent(event);
    if(event->scenePos()==mouseDownPos&&event->button()==Qt::LeftButton&&boundingRect().adjusted(15,10,-15,-10).contains(event->pos())){
		myValue=!myValue;
		foreach(Connection* c, myOutputs){
			if(myValue)c->setValue(High);
			if(!myValue)c->setValue(Low);
		}
		
		update();
    }
}

QRectF Switch::boundingRect() const
{
    return QRectF(-25,-25,width,height);
}

void Switch::setPrivateXml(QXmlStreamWriter *xml){
    xml->writeAttribute("value",myValue?"true":"false");
	xml->writeAttribute("shortcut", shortcut->key().toString());
}

void Switch::readPrivateXml(QXmlStreamReader *xml){
    myValue=(xml->attributes().value("value").toString()=="true"?1:0);
	if(shortcut!=0){
		shortcut->setKey(QKeySequence::fromString(xml->attributes().value("shortcut").toString()));
	}
    foreach(Connection* c, myOutputs){
		c->setValue(myValue);
    }
}

bool Switch::isInput(){
    return true;
}

void Switch::setInput(bool svalue){
    myValue=svalue;
    update();
    foreach(Connection* c, myOutputs){
		c->setValue(myValue);
    }
}

bool Switch::createFormBefore(){
	QKeySequence seq;
	if(shortcut!=0){
		seq=shortcut->key();
	}
	QKeySequenceWidget *key=new QKeySequenceWidget(seq,tr("No Key"));
	QLabel *lab=new QLabel(tr("Shortcut"));
	layout->addRow(lab,key);
	additionalWidgets<<key<<lab;
	connect(key, SIGNAL(keySequenceChanged(QKeySequence)), this, SLOT(changeKeySequence(QKeySequence)));
	return true;
}

void Switch::changeKeySequence(const QKeySequence &seq){
	if(shortcut!=0)
		shortcut->setKey(seq);
}

void Switch::keyTriggered(){
	setInput(!myValue);
}

QVariant Switch::itemChange(GraphicsItemChange change, const QVariant &value){
	QVariant ret=Element::itemChange(change,value);
	if(change==ItemSceneHasChanged){
		if(!value.value<QGraphicsScene*>()==0){
			shortcut=new QShortcut((QWidget*)(static_cast<Scene*>(scene()))->mainWindow());	
			connect(shortcut, SIGNAL(activated()), this, SLOT(keyTriggered()));		
		}
	}
	return ret;
}

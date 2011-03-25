#include "oppositefinder.h"
#include "src/defines.h"
#include "src/scene/scene.h"
#include "element.h"

QList<QColor> OppositeFinder::colors=QList<QColor>();

OppositeFinder::OppositeFinder(Element *eparent, int elementType, QGraphicsObject *parent):
    QGraphicsObject(parent)
{
	pressed=0;
	myOther=0;
	myParentElement=eparent;
	setData(ElementRecognition, "oppositefinder");
	myElementType=elementType;
	myLine=new QGraphicsLineItem;
	myLine->setVisible(false);
	QPen linePen;
	linePen.setColor(QColor(50,50,50));
	linePen.setWidthF(2.5);
	linePen.setCapStyle(Qt::RoundCap);
	myLine->setPen(linePen);
	setAcceptHoverEvents(true);
}

void OppositeFinder::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if(myOther!=0){
		painter->setPen(myColor);
	}
	painter->drawEllipse(boundingRect().center(), 4, 4);
}

QRectF OppositeFinder::boundingRect() const
{
	return QRectF(0,0,9,9);
}

void OppositeFinder::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->button()==Qt::LeftButton){
		static_cast<Scene*>(scene())->highlight(0);
		scene()->addItem(myLine);
		pressed=true;
		myLine->setVisible(true);
		myLine->setLine(QLineF(mapToScene(boundingRect().center()), mapToScene(boundingRect().center())));
	}
}

void OppositeFinder::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if(pressed=true){
		QLineF line;
		line.setP1(mapToScene(boundingRect().center()));
		line.setP2(event->scenePos());
		line.setLength(line.length()-5);
		QGraphicsItem *item=scene()->itemAt(event->scenePos());
		if(item!=0){
			if(item->data(ElementRecognition).toString()=="oppositefinder"){
				OppositeFinder *of=static_cast<OppositeFinder*>(item);
				if(of->myElementType==myElementType){
					line.setP2(of->mapToScene(of->boundingRect().center()));
				}
			}
		}
		myLine->setLine(line);
	}
}

void OppositeFinder::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if(pressed){
		myLine->setVisible(false);
		pressed=false;
		QGraphicsItem *item=scene()->itemAt(event->scenePos());
		if(item!=0){
			if(item->data(ElementRecognition).toString()=="oppositefinder"){
				OppositeFinder *of=static_cast<OppositeFinder*>(item);
				if(of->myElementType==myElementType&&of->myEntrance!=myEntrance){
					makeConnection(of);
				} else {
					makeConnection(0);
				}
			}
		} else {
			makeConnection(0);
		}
	}
}

OppositeFinder * OppositeFinder::otherOppositeFinder()
{
	return myOther;
}

Element * OppositeFinder::otherElement()
{
	if(myOther!=0){
		return myOther->myParentElement;
	} else {
		return 0;
	}
}

Element * OppositeFinder::element()
{
	return myParentElement;
}

QPainterPath OppositeFinder::shape() const
{
	QPainterPath ret;
	ret.addEllipse(boundingRect().center(), 14, 14);
	return ret;
}

void OppositeFinder::setEntrance(ConnectionType type)
{
	myEntrance=type;
}

QColor OppositeFinder::findNewColor()
{
	QColor ret;
	ret.setHsv(0,255,255);
	while(1){
		int hue=rand()%256;
		ret.setHsv(hue, 255, 255);
		if(!colors.contains(ret))
			break;
	}
	colors<<ret;
	return ret;
}

void OppositeFinder::makeConnection(OppositeFinder *other)
{
	if(other==0&&myOther!=0){
		myOther->myOther=0;
		myOther->update();
		myOther=0;
		colors.removeAll(myColor);
	} else if(other!=0) {
		if(myOther!=0){
			makeConnection(0);
		}
		myOther=other;
		myOther->myOther=this;
		myColor=findNewColor();
		myOther->myColor=myColor;
	}
	if(other!=0){
		emit(foundOther(other->element()));
	}
	update();
}

void OppositeFinder::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	if(otherElement()!=0){
		static_cast<Scene*>(scene())->highlight(otherElement()->toGraphicsObject());
	}
}

void OppositeFinder::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	static_cast<Scene*>(scene())->highlight(0);
}


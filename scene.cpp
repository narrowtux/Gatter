#include "scene.h"
#include "element.h"
Scene::Scene(QObject *parent) :
    QGraphicsScene(parent)
{
	rect=0;
}

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
			view->ensureVisible(event->scenePos().x(),event->scenePos().y(),5,5);
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

void Scene::addElement(Element *e){
    addItem(e);
    int maxID=0;
    foreach(int key, elements.keys()){
	if(key>maxID){
	    maxID=key;
	}
    }
    elements.insert(maxID+1,e);
    e->uniqueId=maxID+1;
}

void Scene::removeElement(Element *e){
    //Warning: this does not delete the removed Element!
    removeItem(e);
    elements.remove(e->uniqueId);
}

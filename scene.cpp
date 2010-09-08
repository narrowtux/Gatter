#include "scene.h"

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
	if(pressed){
		if(rect==0){
			rect=addRect(rectFromPoints(startPos,event->scenePos()));
			rect->setBrush(QApplication::palette().highlight());
			rect->setPen(Qt::NoPen);
		} else {
			rect->setRect(rectFromPoints(startPos,event->scenePos()));
		}
		QPainterPath selection;
		selection.addRect(rect->rect());
		setSelectionArea(selection);
	}
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
	if(pressed){
		rect->setRect(rectFromPoints(startPos,event->scenePos()));
		QPainterPath selection;
		selection.addRect(rect->rect());
		removeItem(rect);
		delete rect;
		rect=0;
		pressed=false;
	}
}

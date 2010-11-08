#include "highlighter.h"
#include <QPainter>
Highlighter::Highlighter(QGraphicsObject *parent) :
    QGraphicsObject(parent)
{
}

QRectF Highlighter::boundingRect() const{
	return myBoundingRect;
}

void Highlighter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	painter->setPen(Qt::NoPen);
	QBrush b(QColor(50,50,50,150));
	painter->setBrush(b);
	QPainterPath path;
	path.addRect(myBoundingRect);
	foreach(QGraphicsItem *item, myHighlightItems){
		path.addPath(mapFromItem(item, item->shape()));
	}
	path.setFillRule(Qt::OddEvenFill);
	painter->drawPath(path);
}

void Highlighter::setHighlightArea(QRectF rect){
	myHighlightArea=rect;
	update();
}

void Highlighter::setBoundingRect(QRectF rect){
	myBoundingRect=rect;
	prepareGeometryChange();
	update();
}

void Highlighter::highlight(QGraphicsItem *item){
	myHighlightItems.clear();
	myHighlightItems<<item;
	update();
}

void Highlighter::highlight(QList<QGraphicsItem *> items){
	myHighlightItems=items;
	update();
}

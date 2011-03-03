#include "highlighter.h"
#include <QPainter>
#include <QGraphicsBlurEffect>
/*!
  constructs the Highlighter as a child of the given \a parent.
  */

Highlighter::Highlighter(QGraphicsObject *parent) :
    QGraphicsObject(parent)
{
	QGraphicsBlurEffect *effect=new QGraphicsBlurEffect;
	effect->setBlurRadius(10);
	setGraphicsEffect(new QGraphicsBlurEffect());
}

/*!
  returns the bounding rect
  */

QRectF Highlighter::boundingRect() const{
	return myBoundingRect;
}

void Highlighter::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
	painter->setPen(Qt::NoPen);
	QBrush b(QColor(50,50,50,150));
	painter->setBrush(b);
	QPainterPath path;
	path.addRect(myBoundingRect);
	QPainterPath highlightElements;
	highlightElements.setFillRule(Qt::WindingFill);
	foreach(QGraphicsItem *item, myHighlightItems){
		highlightElements.addPath(mapFromItem(item, item->shape()));
	}
	path.addPath(highlightElements.simplified());
	path.setFillRule(Qt::OddEvenFill);
	painter->drawPath(path);
}

void Highlighter::setHighlightArea(QRectF rect){
	myHighlightArea=rect;
	update();
}


/*!
  sets the \a rect that should be displayed. Usually the bounding rect is the 
  same as the scenes bounding rect.
  */

void Highlighter::setBoundingRect(QRectF rect){
	myBoundingRect=rect;
	prepareGeometryChange();
	update();
}

/*!
  convenience method for calling Highlighter::highlight(QList<QGraphicsItem *>()<<\a item);
  
  If \a item == 0, the highlighter will be hidden.
  */

void Highlighter::highlight(QGraphicsItem *item){
	myHighlightItems.clear();
	myHighlightItems<<item;
	update();
}

/*!
  highlights multiple items
  */

void Highlighter::highlight(QList<QGraphicsItem *> items){
	myHighlightItems=items;
	update();
}

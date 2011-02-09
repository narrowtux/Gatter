#include "breadcumbitem.h"
#include <QtGui>
BreadCumbItem::BreadCumbItem(QWidget *parent) :
    QWidget(parent)
{
	myAction = 0;
	leftDown = false;
}

QSize BreadCumbItem::sizeHint() const
{
	return QSize(100,50);
}

void BreadCumbItem::setAction(QAction *action)
{
	myAction = action;
	repaint();
}

QAction * BreadCumbItem::action()
{
	return myAction;
}

void BreadCumbItem::paintEvent(QPaintEvent *e)
{
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	QPainterPath arrowPath;
	QPolygonF poly;
	poly	<<QPointF(0,0)
	        <<QPointF(width()-10, 0)
	        <<QPointF(width(), height()/2)
	        <<QPointF(width()-10, height())
	        <<QPointF(0,height())
	        <<QPointF(0,0);
	arrowPath.addPolygon(poly);
	p.setPen(Qt::black);
	if(leftDown)
	{
		p.setBrush(Qt::darkGray);
	} else {
		p.setBrush(Qt::gray);
	}
	p.drawPath(arrowPath);
	if(myAction!=0){
		if(leftDown){
			p.setPen(Qt::white);
		}
		p.drawText(QRect(QPoint(0,0), geometry().size()), myAction->text(), QTextOption(Qt::AlignCenter|Qt::AlignVCenter));
	}
}

void BreadCumbItem::mousePressEvent(QMouseEvent *e)
{
	leftDown = e->button()==Qt::LeftButton;
	repaint();
}

void BreadCumbItem::mouseReleaseEvent(QMouseEvent *e)
{
	if(leftDown&&QRect(QPoint(0,0), geometry().size()).contains(e->pos())&&e->button()==Qt::LeftButton)
	{
		if(myAction)myAction->trigger();
		qDebug()<<"Trigger!";
	}
	if(e->button()==Qt::LeftButton){
		leftDown=false;
	}
	repaint();
}

#include "breadcumbitem.h"
#include <QtGui>
BreadCumbItem::BreadCumbItem(QWidget *parent) :
    QWidget(parent)
{
	myAction = 0;
	leftDown = false;
	myLeftItem = myRightItem = 0;
}

QSize BreadCumbItem::sizeHint() const
{
	return QSize(100,20);
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
	if(myRightItem){
		if(myRightItem->leftDown){
			p.setBrush(QColor(94,135,227));
			p.setPen(QColor(149,149,149));
		} else {
			QLinearGradient g;
			g.setColorAt(0,QColor(247,247,247));
			g.setColorAt(1, QColor(235,235,235));
			g.setStart(0,1);
			g.setFinalStop(0,height()-2);
			p.setBrush(g);
			p.setPen(QColor(190,190,190));
		}
		QPolygonF poly;
		poly	<<QPointF(width()-10, 0)
		        <<QPointF(width()+2, 0)
		        <<QPointF(width()+2, height())
		        <<QPointF(width()-10, height())
		        <<QPointF(width(), height()/2)
		        <<QPointF(width()-10, 0);
		QPainterPath path;
		path.addPolygon(poly);
		p.drawPath(path);
	}
	QPainterPath arrowPath;
	QPolygonF poly;
	poly	<<QPointF(0,0)
	        <<QPointF(width()-10, 0)
	        <<QPointF(width(), height()/2)
	        <<QPointF(width()-10, height())
	        <<QPointF(0,height());
	if(!myLeftItem){
		poly<<QPointF(0,0);
	}
	arrowPath.addPolygon(poly);
	if(leftDown)
	{
		p.setBrush(QColor(94,135,227));
		p.setPen(QColor(149,149,149));
	} else {
		QLinearGradient g;
		g.setColorAt(0,QColor(247,247,247));
		g.setColorAt(1, QColor(235,235,235));
		g.setStart(0,1);
		g.setFinalStop(0,height()-2);
		p.setBrush(g);
		p.setPen(QColor(190,190,190));
	}
	
	p.drawPath(arrowPath);
	if(myAction!=0){
		if(leftDown){
			p.setPen(Qt::white);
		} else {
			p.setPen(Qt::black);
		}
		p.drawText(QRect(QPoint(0,0), geometry().size()), myAction->text(), QTextOption(Qt::AlignCenter|Qt::AlignVCenter));
	}
	
	QRect rect = p.boundingRect(QRect(0,0,0,0), myAction->text(), QTextOption(Qt::AlignCenter|Qt::AlignVCenter)).toRect();
	setMinimumSize(rect.size());
}

void BreadCumbItem::mousePressEvent(QMouseEvent *e)
{
	leftDown = e->button()==Qt::LeftButton;
	repaint();
	if(myLeftItem)
		myLeftItem->repaint();
}

void BreadCumbItem::mouseReleaseEvent(QMouseEvent *e)
{
	if(leftDown&&QRect(QPoint(0,0), geometry().size()).contains(e->pos())&&e->button()==Qt::LeftButton)
	{
		if(myAction){
			myAction->trigger();
			qDebug()<<"trigger";
		}
	}
	if(e->button()==Qt::LeftButton){
		leftDown=false;
	}
	repaint();
	if(myLeftItem)
		myLeftItem->repaint();
}

void BreadCumbItem::setLeftBreadCumbItem(BreadCumbItem *left)
{
	myLeftItem = left;
}

void BreadCumbItem::setRightBreadCumbItem(BreadCumbItem *right)
{
	myRightItem = right;
}

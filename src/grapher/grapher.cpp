#include "grapher.h"



Grapher::Grapher(QObject *parent) :
    QObject(parent)
{
	myScene=0;
}

void Grapher::setScene(QGraphicsScene *scene)
{
	myScene=scene;
	update();
}

void Grapher::setPoints(QList<QPointF> points)
{
	Line *l=new Line;
	l->pen=QPen(QColor(Qt::black));
	l->offset=QPointF(0,0);
	l->points=points;
	myLines.append(l);
	update();
}

void Grapher::update()
{
	myScene->clear();
	if(myScene){
		foreach(Line* line, myLines){
			QPointF lastPoint = line->points.at(0);
			foreach(QPointF p, line->points){
				QLineF l(lastPoint, p);
				QTransform t;
				t.translate(line->offset.x(), line->offset.y());
				t.scale(line->scaleX, line->scaleY);
				l=t.map(l);
				line->lastLine=myScene->addLine(l, line->pen);
				lastPoint=p;
			}
		}
	} else {
		qDebug()<<"Can't draw: No Scene!";
	}
}

void Grapher::addLine(Grapher::Line *l)
{
	myLines<<l;
	update();
}

void Grapher::clear()
{
	myLines.clear();
	update();
}

Grapher::Line::Line()
{
	scaleX=1;
	scaleY=1;
	offset=QPoint(0,0);
	lastLine=0;
}

void Grapher::addPoint(Grapher::Line *line, QPointF p)
{
	QPointF lastPoint=line->points.last();
	line->points<<p;
	QLineF l(lastPoint, p);
	QTransform t;
	t.translate(line->offset.x(), line->offset.y());
	t.scale(line->scaleX, line->scaleY);
	l=t.map(l);
	line->lastLine=myScene->addLine(l, line->pen);
}

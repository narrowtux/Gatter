#include "grapher.h"


#include "src/elements/element.h"
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
	qreal maxX;
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
				maxX=qMax(maxX, l.p2().x());
				line->lastLine=myScene->addLine(l, line->pen);
				lastPoint=p;
			}
		}
		updateLabel(maxX);
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
	textItem=0;
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
	updateLabel(l.p2().x());
}

void Grapher::updateLabel(qreal x)
{
	QList<QGraphicsItem*> items;
	foreach(Line * line, myLines){
		line->updateLabelText();
		QGraphicsTextItem *text=line->textItem;
		if(text==0){
			line->textItem=new QGraphicsTextItem;
			myScene->addItem(line->textItem);
			text=line->textItem;
		}
		items<<text;
		text->setPlainText(line->labelName);
		text->setToolTip(line->labelTooltip);
		text->setPos(x+20, line->offset.y());
	}
	QPainterPath path;
	foreach(QGraphicsItem *item, items){
		path.addRect(item->boundingRect());
	}
	if(items.count())
		path.translate(items.at(0)->pos());
	qDebug()<<path.boundingRect();
	myScene->views().at(0)->ensureVisible(path.boundingRect());
}

void Grapher::Line::updateLabelText()
{
	labelName=connection->name();
	QString name=connection->name();
	if(name==""){
		name=tr("(No name)");
		labelName=tr("(No name)");
	}
	name+=tr(" on %0").arg(connection->element()->metaObject()->className());
	if(connection->connectionType()==Input){
		name+=" ("+tr("Input")+")";
	} else {
		name+=" ("+tr("Output")+")";
	}
	labelTooltip=name;
}

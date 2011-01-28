#include "sevensegmentoutput.h"

SevenSegmentOutput::SevenSegmentOutput(QGraphicsObject *parent) :
		Element(parent)
{
    width=50;
    height=50;
    minHeight=50;
    setMinMaxInputsOutputs(7,7,0,0);
    for (int i=0;i<7;i++){
		values<<false;
    }
    myType="hexoutput";
}

void SevenSegmentOutput::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter,option,widget);
    /*
     Seven-Segment Display:
        A0
     F5    B1
        G6
     E4    C2
        D3
     */
    QList<QPair<int, QPointF> > segmentTransformations;
	segmentTransformations
			<<QPair<int, QPointF>(0, QPointF(0,-10))
			<<QPair<int, QPointF>(90, QPointF(5,-5))
			<<QPair<int, QPointF>(90, QPointF(5,5))
			<<QPair<int, QPointF>(180, QPointF(0,10))
			<<QPair<int, QPointF>(270, QPointF(-5,5))
			<<QPair<int, QPointF>(270, QPointF(-5,-5))
			<<QPair<int, QPointF>(0, QPointF(0,0));
	QPolygonF segment=segmentPolygon();
    QPair<int, QPointF> cur;
	int i=0, scale=3;
	painter->setPen(QColor("white"));
	foreach(cur, segmentTransformations){
		if(values[i]){
			painter->setBrush(QColor("red"));
		}else{
			painter->setBrush(QColor("lightgray"));
		}
		QPainterPath path;
		path.addPolygon(segment);
		
		QTransform trans;
		trans.rotate(cur.first);
		trans.scale(scale,scale);
		path=trans.map(path);
		path.translate(cur.second*scale);
		path.translate(boundingRect().center());
		painter->drawPath(path);
		i++;
	}
}

QRectF SevenSegmentOutput::boundingRect() const{
    return QRectF(-25,-25,width,height);
}

void SevenSegmentOutput::recalculate(){
    for (int i=0;i<7;i++){
		values[i]=myInputs[i]->value();
    }
    update();
}

QPolygonF SevenSegmentOutput::segmentPolygon(){
	QPolygonF out;
	out<<QPointF(-4,-1)
			<<QPointF(4,-1)
			<<QPointF(5,0)
			<<QPointF(4,1)
			<<QPointF(-4,1)
			<<QPointF(-5,0)
			<<QPointF(-4,-1);
	return out;
}

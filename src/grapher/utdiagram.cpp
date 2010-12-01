#include "utdiagram.h"

UTDiagram::UTDiagram(QWidget *parent) :
    QGraphicsView(parent)
{
	myScene=new QGraphicsScene;
	setScene(myScene);
	scale(1,-1);
	myGrapher=new Grapher;
	myGrapher->setScene(myScene);
	time.setHMS(0,0,0);
	time.start();
	timer=new QTimer;
	timer->setInterval(1000.0/10);
	timer->start();
	timeOffset=0;
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	recording=true;
	setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
}

void UTDiagram::changeSignal(bool value)
{
	if(recording){
		QObject * obj=sender();
		if(!myLines.keys().contains(obj)){
			myLine=new Grapher::Line;
			myLine->points<<QPointF(0,0);
			myLine->scaleY=20;
			myLine->scaleX=50;
			myLine->offset=QPointF(0.5,0.5-30.0*myLines.count());
			myGrapher->addLine(myLine);
			myLines.insert(obj, myLine);
		}
		myLine=myLines.value(obj);
		QPointF p=myLine->points.last();
		qreal t=(time.elapsed()+timeOffset)/1000.0;
		p.setX(t);
		myGrapher->addPoint(myLine, p);
		myGrapher->addPoint(myLine, QPointF(t, (int)value));
		myGrapher->addPoint(myLine, QPointF(t, (int)value));
	}
}

void UTDiagram::startRecording()
{
	setRecordingStatus(true);
}

void UTDiagram::stopRecording()
{
	setRecordingStatus(false);
}

void UTDiagram::setRecordingStatus(bool rec)
{
	recording=rec;
    if(rec){
		timer->start();
		time.restart();
	} else {
		timer->stop();
		timeOffset+=time.elapsed();
	}
}

void UTDiagram::updateGraph()
{
	myGrapher->update();
}

void UTDiagram::tick()
{
	foreach(Grapher::Line *line, myLines){
		QGraphicsLineItem* lastLine=line->lastLine;
		if(lastLine){
			QLineF l=lastLine->line();
			QPointF p=l.p2();
			p.setX((qreal)(time.elapsed()+timeOffset)/1000.0*50.0);
			l.setP2(p);
			lastLine->setLine(l);
			ensureVisible(lastLine);
		}
	}
}

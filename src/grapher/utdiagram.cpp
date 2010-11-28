#include "utdiagram.h"

UTDiagram::UTDiagram(QWidget *parent) :
    QGraphicsView(parent)
{
	myScene=new QGraphicsScene;
	setScene(myScene);
	scale(1,-1);
	myGrapher=new Grapher;
	myGrapher->setScene(myScene);
	myLine=new Grapher::Line;
	myLine->points<<QPointF(0,0);
	myLine->scaleY=50;
	myLine->scaleX=50;
	myLine->offset=QPointF(0.5,0.5);
	myGrapher->addLine(myLine);
	time.setHMS(0,0,0);
	time.start();
	timer=new QTimer;
	timer->setInterval(1000.0/10);
	timer->start();
	timeOffset=0;
	connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
	recording=true;
}

void UTDiagram::changeSignal(bool value)
{
	if(recording){
		QPointF p=myLine->points.last();
		qreal t=(time.elapsed()+timeOffset)/1000.0;
		p.setX(t);
		myLine->points<<p;
		myLine->points<<QPointF(t, (int)value);
		myLine->points<<QPointF(t, (int)value);
		myGrapher->update();
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
	QGraphicsLineItem* lastLine=myLine->lastLine;
	if(lastLine){
		QLineF l=lastLine->line();
		QPointF p=l.p2();
		p.setX((qreal)(time.elapsed()+timeOffset)/1000.0*50.0);
		l.setP2(p);
		lastLine->setLine(l);
		ensureVisible(lastLine);
	}
}

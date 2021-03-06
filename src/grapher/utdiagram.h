#ifndef UTDIAGRAM_H
#define UTDIAGRAM_H

#include <QWidget>
#include <QtGui>
#include "grapher.h"

class UTDiagram : public QGraphicsView
{
    Q_OBJECT
public:
    explicit UTDiagram(QWidget *parent = 0);
	~UTDiagram();
signals:

public slots:
	void changeSignal(bool value);
	void startRecording();
	void stopRecording();
	void setRecordingStatus(bool rec);
	void updateGraph();
	void setAutoScrollStatus(bool autoScroll);
private:
private slots:
	void tick();
private:
	Grapher *myGrapher;
	QGraphicsScene *myScene;
	Grapher::Line *myLine;
	QTime time;
	int timeOffset;
	QTimer *timer;
	bool recording;
	QMap<QObject *, Grapher::Line *> myLines;
	bool myAutoScroll;
};

#endif // UTDIAGRAM_H

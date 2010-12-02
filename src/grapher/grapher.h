#ifndef GRAPHER_H
#define GRAPHER_H

#include <QObject>
#include <QtGui>
#include "src/connection.h"

class Grapher : public QObject
{
    Q_OBJECT
public:
	class Line;
	explicit Grapher(QObject *parent = 0);
	void setScene(QGraphicsScene * scene);
	void setPoints(QList<QPointF> points);
	void addLine(Line* l);
	void clear();
	void update();
	void addPoint(Line *l, QPointF p);
	void updateLabel(qreal x);
	void setAutoScrollStatus(bool autoScroll);
signals:
	
public slots:
	
private:
	QGraphicsScene * myScene;
	QList<Line*> myLines;
	QList<QGraphicsItem*> itemsToClear;
	bool myAutoScroll;
};

class Grapher::Line{
public:
	Line();
	QList<QPointF> points;
	QPen pen;
	QPointF offset;
	qreal scaleX, scaleY;
	QGraphicsLineItem *lastLine;
	QString labelName;
	QString labelTooltip;
	Connection* connection;
	QGraphicsTextItem *textItem;
	void updateLabelText();
private:
};
#endif // GRAPHER_H

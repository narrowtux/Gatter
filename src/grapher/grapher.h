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
signals:
	
public slots:
	
private:
	QGraphicsScene * myScene;
	QList<Line*> myLines;
};

class Grapher::Line{
	friend class Grapher;
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
private:
	QGraphicsTextItem *textItem;
	void updateLabelText();
};
#endif // GRAPHER_H

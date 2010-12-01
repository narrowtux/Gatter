#ifndef GRAPHER_H
#define GRAPHER_H

#include <QObject>
#include <QtGui>

class Grapher : public QObject
{
    Q_OBJECT
	
	
public:
	struct Line{
		Line();
		QList<QPointF> points;
		QPen pen;
		QPointF offset;
		qreal scaleX, scaleY;
		QGraphicsLineItem *lastLine;
	};
    explicit Grapher(QObject *parent = 0);
	void setScene(QGraphicsScene * scene);
	void setPoints(QList<QPointF> points);
	void addLine(Line* l);
	void clear();
	void update();
	void addPoint(Line *l, QPointF p);
signals:
	
public slots:
	
private:
	QGraphicsScene * myScene;
	QList<Line*> myLines;
};

#endif // GRAPHER_H

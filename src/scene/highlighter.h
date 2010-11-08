#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QGraphicsObject>
#include <QGraphicsScene>

class Highlighter : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Highlighter(QGraphicsObject *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setHighlightArea(QRectF rect);
	void setBoundingRect(QRectF rect);
	void highlight(QGraphicsItem *item);
	void highlight(QList<QGraphicsItem*> item);
signals:

public slots:
private:
	QRectF myHighlightArea, myBoundingRect;
	QList<QGraphicsItem*> myHighlightItems;
};

#endif // HIGHLIGHTER_H

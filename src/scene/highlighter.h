#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QGraphicsObject>
#include <QGraphicsScene>

/*!
  \class Highlighter
  \brief Highlighter can highlight one or multiple QGraphicsItems in a scene.
  
  The scene will be darkened and the highlighted items will shine through it.
  
  \code
  QGraphicsScene *scene = new QGraphicsScene;
	Highlighter *highlighter = new Highlighter();
  scene->addItem(highlighter);
  QGraphicsRectItem *item = new QGraphicsRectItem(20,10);
  scene->addItem(item);
  highlighter->setBoundingRect(scene->boundingRect());
  highlighter->highlight(item);
  //to unhighlight, call
  highlighter->highlight(0);
  \endcode
  */

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

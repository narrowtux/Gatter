#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H
#include "src/elements/element.h"

/*!
  \class Distributor
  \brief A Distributor is an Element that passes its input to all of its outputs.
  
  On the scene, it's represented as a dot.
  */
class Distributor : public Element
{
    Q_OBJECT
public:
	/*! Constructs the Distributor as a child of \a parent. */
    explicit Distributor(QGraphicsObject *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QPainterPath shape() const;
signals:

public slots:
protected:
	void relayoutConnections();
	void recalculate();
};

#endif // DISTRIBUTOR_H

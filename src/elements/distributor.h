#ifndef DISTRIBUTOR_H
#define DISTRIBUTOR_H
#include "src/elements/element.h"

class Distributor : public Element
{
    Q_OBJECT
public:
    explicit Distributor(QGraphicsObject *parent = 0);
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:

public slots:
protected:
	void relayoutConnections();
	void recalculate();
};

#endif // DISTRIBUTOR_H

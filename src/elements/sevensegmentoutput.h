#ifndef HEXOUTPUT_H
#define HEXOUTPUT_H
#include "src/elements/element.h"

class SevenSegmentOutput : public Element
{
    Q_OBJECT
public:
    explicit SevenSegmentOutput(QGraphicsObject *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    void recalculate();
    QList<bool> values;
	QPolygonF segmentPolygon();
signals:

public slots:

};

#endif // HEXOUTPUT_H

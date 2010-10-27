#ifndef HEXOUTPUT_H
#define HEXOUTPUT_H
#include "src/elements/element.h"

class HexOutput : public Element
{
    Q_OBJECT
public:
    explicit HexOutput(QGraphicsObject *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    void recalculate();
    QList<bool> values;
signals:

public slots:

};

#endif // HEXOUTPUT_H

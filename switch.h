#ifndef SWITCH_H
#define SWITCH_H

#include "defines.h"
#include "element.h"

class Switch : public Element
{
    Q_OBJECT
public:
    explicit Switch(QObject *parent = 0);
protected:
    bool value;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QRectF boundingRect() const;
    QPointF mouseDownPos;
signals:

public slots:

};

#endif // SWITCH_H

#ifndef BUTTON_H
#define BUTTON_H
#include "src/elements/switch.h"
class Button : public Switch
{
    Q_OBJECT
public:
    explicit Button(QGraphicsObject *parent = 0);

signals:

private slots:
    void updateValue();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF startPos;
    QTimer delay;
};

#endif // BUTTON_H

#ifndef DELAY_H
#define DELAY_H
#include "src/elements/element.h"

/*!
  \class Delay
  \brief Delay provides an Element that delays a signal.
  */
class Delay : public Element
{
    Q_OBJECT
public:
    explicit Delay(QGraphicsObject *parent = 0);
    QRectF boundingRect() const;
signals:

protected:
    bool value;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void recalculate();
    QTimer* timer;
private slots:
    void sendChanges();
};

#endif // DELAY_H

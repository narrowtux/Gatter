#ifndef LAMP_H
#define LAMP_H
#include "defines.h"
#include "element.h"

class Lamp : public Element
{
    Q_OBJECT
public:
    explicit Lamp(QObject *parent = 0);
    
signals:
private slots:
    void setColor(QColor c);
protected:
    QRectF boundingRect() const;
    bool value;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QColor color;
    void recalculate();
    void createFormBefore();
};

#endif // LAMP_H

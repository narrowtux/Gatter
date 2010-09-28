#ifndef GATTER_H
#define GATTER_H
#include "element.h"
#include "defines.h"
class Gatter : public Element
{
    Q_OBJECT
public:
    explicit Gatter(QObject *parent = 0);
    enum Type{AND,NOT,OR,XOR,MULTIPLEXER};
    QSize sizeHint() const;
    QRectF boundingRect() const;
    void setType(Type t);
    Type type();
    void recalculate();
signals:

public slots:
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    Type myType;
    bool beforeValue;
    bool beforeUndefined;
};

#endif // GATTER_H

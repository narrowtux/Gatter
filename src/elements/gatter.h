#ifndef GATTER_H
#define GATTER_H
#include "element.h"
#include "defines.h"
#include <QTimer>
class Gatter : public Element
{
    Q_OBJECT
public:
    explicit Gatter(QGraphicsObject *parent = 0);
    enum Type{AND,NOT,OR,XOR,DUPLICATOR,ICON};
    QSize sizeHint() const;
    QRectF boundingRect() const;
    void setType(Type t);
    Type type();
    void recalculate();
    static int delayMS;
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
signals:
    void changed();
public slots:
    void sendChanges();
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    Type myGatterType;
    bool beforeValue;
    bool beforeUndefined;
    QTimer* delay;
};

#endif // GATTER_H

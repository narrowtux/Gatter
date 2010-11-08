#ifndef LAMP_H
#define LAMP_H
#include "src/defines.h"
#include "src/elements/element.h"

class Lamp : public Element
{
    Q_OBJECT
public:
    explicit Lamp(QGraphicsObject *parent = 0);
    bool isOutput();
    QPainterPath shape() const;
signals:
private slots:
    void setColor(QColor c);
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QColor color;
    void recalculate();
    bool createFormBefore();
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
};

#endif // LAMP_H

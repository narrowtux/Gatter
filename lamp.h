#ifndef LAMP_H
#define LAMP_H
#include "defines.h"
#include "element.h"

class Lamp : public Element
{
    Q_OBJECT
public:
    explicit Lamp(QObject *parent = 0);
    bool isOutput();
    
signals:
private slots:
    void setColor(QColor c);
protected:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QColor color;
    void recalculate();
    void createFormBefore();
    void setPrivateXml(QCoreXmlStreamWriter *xml);
    void readPrivateXml(QCoreXmlStreamReader *xml);
};

#endif // LAMP_H

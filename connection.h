#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QGraphicsItem>
#include "defines.h"
class Element;

class Connection : public QObject, public QGraphicsItem
{
    Q_OBJECT
friend class Element;
public:
    explicit Connection(QObject *parent = 0);
    ~Connection();
    void setNegated(bool n);
    bool isNegated();
    bool value();
    void setValue(bool v);
    QRectF boundingRect() const;
signals:
    void changed(bool);
protected:
    bool myValue;
    bool myNegated;
    bool lastValue;
    
    
    //For making connections
    void setOther(Connection* other);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    bool pressed;
    bool poked;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    ConnectionType myConnectionType;
    QGraphicsLineItem* line;
    QPointF startPos;
    Connection* connectedTo;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void poke(bool in);
    Connection* lastI;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void updateLine();
};

#endif // CONNECTION_H

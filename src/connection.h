#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QGraphicsItem>
#include "src/defines.h"
#include "src/connectionline.h"
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
    bool isConnected();
    void setName(QString name);
    QString name();
    Element* element();
    void setElement(Element* e);
    Connection* connectedTo();
    void connectWith(Connection* c);
    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);
    void setClock(bool clock);
	ConnectionType connectionType();
	QList<QGraphicsItem *> connectedElements(QList<QGraphicsItem *> before=QList<QGraphicsItem *>());
signals:
    void changed(bool);
    void recalculate();
protected:
    bool myValue;
    bool myNegated;
    bool lastValue;
    bool myClock;
    QString myName;
    QGraphicsTextItem* label;
    Element* myElement;
    //For making connections
    void setOther(Connection* other);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    bool pressed;
    bool poked;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    ConnectionType myConnectionType;
    ConnectionLine* line;
    QPointF startPos;
    Connection* myConnectedTo;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void poke(bool in);
    Connection* lastI;
	bool lastV;
	bool lastVInit;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void updateLine(QPointF to=QPointF(), Connection*c=0);
};

#endif // CONNECTION_H

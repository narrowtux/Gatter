#ifndef ELEMENT_H
#define ELEMENT_H
#include <QGraphicsItem>
#include <QObject>
#include "scene.h"
#include "defines.h"
#include "connection.h"

class Element : public QObject, public QGraphicsItem
{
    Q_OBJECT
    friend class Scene;
public:
    explicit Element(QObject *parent = 0, QGraphicsItem *gparent=0);
    static QPen getSelectionPen();
    QMap<QString,QVariant> settings();
    void setSettings(QMap<QString, QVariant> s);
signals:

private slots:
    void inputChanged();
protected:
    bool pressed;
    QList<Connection*> myInputs;
    QList<Connection*> myOutputs;
    void addInput(Connection* i);
    void addOutput(Connection* o);
    void removeInput(Connection* i);
    void removeOutput(Connection* o);
    qreal height, width;
    virtual void recalculate();
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    int uniqueId;
    void relayoutConnections();
};

#endif // ELEMENT_H

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include "defines.h"
#include <qxmlstream.h>
class Element;
class MainWindow;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    ~Scene();
    QRectF rectFromPoints(QPointF p1, QPointF p2);
    static QColor highValueColor;
    void addElement(Element* e, int uniqueId=-1);
    void removeElement(Element* e);
    void removeItem(QGraphicsItem *item);
    bool isElement(QGraphicsItem* item);
    void setMainWindow(MainWindow* m);
    void setScale(qreal scale);
    void save(QString fileName);
    void load(QString fileName);
    Element* getElementFromTypeName(QString typeName);
    void connectItems(int inElement, int outElement, int input, int output);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    bool pressed;
    QPointF startPos;
    QGraphicsRectItem* rect;
    QMap<int, Element*> elements;
    MainWindow* myMainWindow;
signals:
	void modified();
public slots:
};

#endif // SCENE_H

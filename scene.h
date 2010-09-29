#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include "defines.h"
class Element;
class MainWindow;

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
	QRectF rectFromPoints(QPointF p1, QPointF p2);
	void addElement(Element* e);
	void removeElement(Element* e);
	void removeItem(QGraphicsItem *item);
	bool isElement(QGraphicsItem* item);
	void setMainWindow(MainWindow* m);
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

public slots:
};

#endif // SCENE_H

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QtGui>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
	QRectF rectFromPoints(QPointF p1, QPointF p2);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	bool pressed;
	QPointF startPos;
	QGraphicsRectItem* rect;
signals:

public slots:

};

#endif // SCENE_H

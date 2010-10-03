#ifndef BUTTON_H
#define BUTTON_H
#include "switch.h"
class Button : public Switch
{
    Q_OBJECT
public:
    explicit Button(QObject *parent = 0);

signals:

private slots:
    void updateValue();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    QPointF startPos;
    QTimer delay;
};

#endif // BUTTON_H

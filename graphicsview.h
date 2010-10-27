#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGesture>
#include <QGestureEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);
    void setScale(qreal scale);
	qreal scaleFactor();
signals:

public slots:
private:
    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent* event);
    
    qreal myScaleFactor;
    qreal currentStepScaleFactor;
};

#endif // GRAPHICSVIEW_H

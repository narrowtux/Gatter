#include "graphicsview.h"
#include "scene.h"

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    grabGesture(Qt::PinchGesture);
    scaleFactor=1;
    currentStepScaleFactor=1;
}

bool GraphicsView::event(QEvent *event){
    if(event->type()==QEvent::Gesture){
	return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    return QGraphicsView::event(event);
}

bool GraphicsView::gestureEvent(QGestureEvent *event){
    if(event->gesture(Qt::PinchGesture)!=0){
	QPinchGesture* gesture=static_cast<QPinchGesture*>(event->gesture(Qt::PinchGesture));
	QPinchGesture::ChangeFlags changeFlags = gesture->changeFlags();
	if (changeFlags & QPinchGesture::RotationAngleChanged) {
	    qreal value = gesture->property("rotationAngle").toReal();
	    qreal lastValue = gesture->property("lastRotationAngle").toReal();
	    foreach(QGraphicsItem*i, scene()->selectedItems()){
		qreal angle=i->data(QGraphicsItem::UserType+1411).toReal()+value-lastValue;
		i->setData(QGraphicsItem::UserType+1411,angle);
		i->rotate(value-lastValue);
	    }
	}
	if (changeFlags & QPinchGesture::ScaleFactorChanged) {
	    qreal value = gesture->property("scaleFactor").toReal();
	    if(qAbs(1.0-value)>0.05){
		currentStepScaleFactor = value;
	    }
	}
	if (gesture->state() == Qt::GestureFinished) {
	    scaleFactor *= currentStepScaleFactor;
	    currentStepScaleFactor = 1;
	    foreach(QGraphicsItem*i, scene()->selectedItems()){
		qreal rotation=i->data(QGraphicsItem::UserType+1411).toReal();
		qreal r=round(rotation/90);
		r*=90;
		qDebug()<<"rotation"<<rotation<<"final rotation"<<r;
		i->rotate(r-rotation);
		i->setData(QGraphicsItem::UserType+1411,r);
	    }
	}
	setScale(scaleFactor);
    }
    return true;
}

void GraphicsView::setScale(qreal scale){
    scaleFactor=scale;
    QTransform tr;
    tr.scale(currentStepScaleFactor * scaleFactor,currentStepScaleFactor * scaleFactor);
    setTransform(tr);
}

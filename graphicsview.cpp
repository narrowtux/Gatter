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
//	if (changeFlags & QPinchGesture::RotationAngleChanged) {
//	    qreal value = gesture->property("rotationAngle").toReal();
//	    qreal lastValue = gesture->property("lastRotationAngle").toReal();
//	    rotationAngle += value - lastValue;
//	}
	if (changeFlags & QPinchGesture::ScaleFactorChanged) {
	    qreal value = gesture->property("scaleFactor").toReal();
	    currentStepScaleFactor = value;
	}
	if (gesture->state() == Qt::GestureFinished) {
	    scaleFactor *= currentStepScaleFactor;
	    currentStepScaleFactor = 1;
	}
	scale(currentStepScaleFactor * scaleFactor, currentStepScaleFactor * scaleFactor);
    }
    return true;
}

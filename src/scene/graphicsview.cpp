
#include "src/scene/graphicsview.h"

#include "src/scene/scene.h"
#include "src/defines.h"
#include "src/elements/element.h"
#include "src/widgets/mainwindow.h"
#include <QPropertyAnimation>
#include "src/elements/subscene.h"
GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{
    grabGesture(Qt::PinchGesture);
    myScaleFactor=1;
    currentStepScaleFactor=1;
	setRenderHint(QPainter::Antialiasing, true);
	setFrameStyle(0);
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
				qreal angle=i->rotation()+value-lastValue;
				i->setData(ElementRotation,angle);
				i->setRotation(angle);
			}
		}
		if (changeFlags & QPinchGesture::ScaleFactorChanged) {
			qreal value = gesture->property("scaleFactor").toReal();
			if(qAbs(1.0-value)>0.05){
				currentStepScaleFactor = value;
			}
			setScale(myScaleFactor);
		}
		if (gesture->state() == Qt::GestureFinished) {
			myScaleFactor *= currentStepScaleFactor;
			currentStepScaleFactor = 1;
			foreach(QGraphicsItem*i, scene()->selectedItems()){
				qreal rotation=i->rotation();
				qreal r=round(rotation/Element::rotationSteps);
				r*=Element::rotationSteps;
				//qDebug()<<"rotation"<<rotation<<"final rotation"<<r;
				QGraphicsObject *obj = static_cast<QGraphicsObject *>(i);
				if(obj!=0){
					QPropertyAnimation *anim = new QPropertyAnimation(this);
					anim->setTargetObject(obj);
					anim->setPropertyName("rotation");
					anim->setStartValue(rotation);
					anim->setEndValue(r);
					anim->setDuration(300);
					anim->start(QAbstractAnimation::DeleteWhenStopped);
				} else {
					i->setRotation(r);
				}
				i->setData(ElementRotation,r);
			}
		}
		setScale(myScaleFactor);
    }
    return true;
}

void GraphicsView::setScale(qreal scale){
	if(scale>8){
		scale = 8;
	}
    myScaleFactor=scale;
    QTransform tr;
    tr.scale(currentStepScaleFactor * myScaleFactor,currentStepScaleFactor * myScaleFactor);
    setTransform(tr);
	emit(scaleFactorChanged(myScaleFactor*100));
}

qreal GraphicsView::scaleFactor(){
	return myScaleFactor;
}

MainWindow * GraphicsView::mainWindow()
{
	return myMainWindow;
}

void GraphicsView::setMainWindow(MainWindow *mainWindow)
{
	myMainWindow = mainWindow;
}

void GraphicsView::setScene(QGraphicsScene *scene, bool notifyMainWindow, SubScene* subscene)
{
	QGraphicsView::setScene(scene);
	if(notifyMainWindow){
		myMainWindow->setScene(static_cast<Scene *>(scene));
		if(subscene!=0){
			if(subscene->containingScene()==scene)
				myMainWindow->breadCumbBar()->addAction(subscene->action());
		}
	}
}

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGesture>
#include <QGestureEvent>

class MainWindow;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);
    void setScale(qreal scale);
	qreal scaleFactor();
	MainWindow *mainWindow();
	void setMainWindow(MainWindow *mainWindow);
	void setScene(QGraphicsScene *scene, bool notifyMainWindow = false);
signals:
	void scaleFactorChanged(int);
public slots:
private:
    bool event(QEvent *event);
    bool gestureEvent(QGestureEvent* event);
    
    qreal myScaleFactor;
    qreal currentStepScaleFactor;
	MainWindow *myMainWindow;
};

#endif // GRAPHICSVIEW_H

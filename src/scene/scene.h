#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QtGui>
#include <QGestureEvent>
#include "highlighter.h"

class QXmlStreamReader;
class QXmlStreamWriter;
class Element;
class MainWindow;
class GraphicsView;
class SubScene;

/*!
  \class Scene
  \brief Scene contains the simulation Elements
  */

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    friend class GraphicsView;
    friend class SubScene;
    explicit Scene(QObject *parent = 0);
    ~Scene();
    QRectF rectFromPoints(QPointF p1, QPointF p2);
    static QColor highValueColor;
    static bool debugMethods;
    void addElement(Element* e, int uniqueId=-1);
    void removeElement(Element* e);
    void removeItem(QGraphicsItem *item);
    bool isElement(QGraphicsItem* item);
    void setMainWindow(MainWindow* m);
    void setScale(qreal scale);
    void save(QString fileName, QXmlStreamWriter* xml=0, QList<Element*> selectionElements=QList<Element*>());
    void load(QString fileName, QXmlStreamReader* xml=0, bool setAllAttributes=true, bool paste=false, QPointF pasteTo=QPointF());
    Element* getElementFromTypeName(QString typeName);
    void connectItems(int inElement, int outElement, int input, int output);
    bool isBlank();
    bool isLoading();
    MainWindow* mainWindow();
    QPointF lastMousePos;
    void paste(const QMimeData* mimeData, QPointF pos);
    QString copy(QList<Element*> myElements);
	void highlight(QGraphicsItem *element);
	void highlight(QList<QGraphicsItem*> myElements);
	QList<Element*> elementList();
	void setCurrentOpenSubScene(SubScene *subscene);
	void setSubscene(SubScene* sub);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    bool pressed;
    QPointF startPos;
    QGraphicsItem* movingItem;
    QMap<int, Element*> myElements;
    MainWindow* myMainWindow;
    bool wantsToDrag;
    bool blank;
    bool loads;
    void dropEvent(QGraphicsSceneDragDropEvent *event);
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event);
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event);
	Highlighter *myHighlighter;
	SubScene *subscene;
signals:
	/*!
	  This signal is emitted whenever an Element is being added, removed or moved.
	  */
    void modified();
	/*!
	  This signal is emitted whenever an Element is being added or removed.
	  */
    void elementAddedOrRemoved();
	
	/*!
	  This signal is emitted whenever an Element moved.
	  
	  \a element contains a list with the moved elements
	  
	  \a oldPos contains a list with the old positions of the elements
	  */
    void elementMoved(QList<Element*> element, QList<QPointF> oldPos);
public slots:
    void clear();
	void clearHighlight();
};

#endif // SCENE_H

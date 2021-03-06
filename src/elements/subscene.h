#ifndef SUBSCENE_H
#define SUBSCENE_H
#include "src/elements/element.h"
#include "src/scene/scene.h"
#include "src/widgets/mainwindow.h"
#include <QGraphicsProxyWidget>
#include <QPushButton>
class SubSceneInfo;

class SubScene : public Element
{
friend class SubSceneInfo;
    Q_OBJECT
public:
    explicit SubScene(QGraphicsObject *parent = 0, bool createMainWindow=true);
    QRectF boundingRect() const;
    void loadFromFile(QString file, bool setAllAttributes=true);
    ~SubScene();
    void setFileName(QString fileName);
    void setInfo(SubSceneInfo* info);
    void setMainWindow(MainWindow*m);
	void setAction(QAction *action);
	QAction *action();
	void setChildSubScene(SubScene *subscene);
	SubScene *childSubScene();
	void setParentSubScene(SubScene *subscene);
	SubScene *parentSubScene();
	Scene *containingScene();
	bool isOpen();
public slots:
	void close();
	void save();
	void reload();
	void returnTo();
	void recalcOutputs();
signals:
	void changed();
private slots:
    void updateConnections();
    void selectFile();
protected:
	void loadEvent();
private:
    Scene* myScene;
	Scene* myContainerScene;
    MainWindow* myMainWindow;
    SubSceneInfo* mySubSceneInfo;
    bool createFormBefore();
    void recalculate();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
    QList<Element*> sceneInputs;
    QList<Element*> sceneOutputs;
    QString fileName;
    QList<bool> inValues, outValues;
	QGraphicsProxyWidget *myButtonsProxy;
	QPushButton *mySaveButton, *myCloseButton;
	bool myOpen;
	static QList<SubScene*> subScenes;
	QAction *myAction;
	SubScene *myChildSubScene;
	SubScene *myParentSubScene;
	GraphicsView *myView;
};

#endif // SUBSCENE_H

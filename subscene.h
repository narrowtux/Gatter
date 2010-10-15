#ifndef SUBSCENE_H
#define SUBSCENE_H
#include "element.h"
#include "scene.h"
#include "mainwindow.h"
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
signals:
private slots:
    void updateConnections();
    void selectFile();
private:
    Scene* myScene;
    MainWindow* myMainWindow;
    SubSceneInfo* mySubSceneInfo;
    void createFormBefore();
    void recalculate();
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPrivateXml(QXmlStreamWriter *xml);
    void readPrivateXml(QXmlStreamReader *xml);
    QList<Element*> sceneInputs;
    QList<Element*> sceneOutputs;
    QString fileName;
    QList<bool> inValues, outValues;
};

#endif // SUBSCENE_H

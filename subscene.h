#ifndef SUBSCENE_H
#define SUBSCENE_H
#include "element.h"
#include "scene.h"
#include "mainwindow.h"

class SubScene : public Element
{
    Q_OBJECT
public:
    explicit SubScene(QObject *parent = 0, bool createMainWindow=true);
    QRectF boundingRect() const;
    void loadFromFile(QString file, bool setAllAttributes=true);
    ~SubScene();
    friend class SubSceneInfo;
    void setFileName(QString fileName);
signals:
private slots:
    void updateConnections();
    void selectFile();
private:
    Scene* myScene;
    MainWindow* myMainWindow;
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

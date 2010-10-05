#ifndef SUBSCENE_H
#define SUBSCENE_H
#include "element.h"
#include "scene.h"
#include "mainwindow.h"

class SubScene : public Element
{
    Q_OBJECT
public:
    explicit SubScene(QObject *parent = 0);
    QRectF boundingRect() const;
    void loadFromFile(QString file);
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
    void setPrivateXml(QCoreXmlStreamWriter *xml);
    void readPrivateXml(QCoreXmlStreamReader *xml);
    QList<Element*> sceneInputs;
    QList<Element*> sceneOutputs;
    QString fileName;
};

#endif // SUBSCENE_H

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

signals:

public slots:
private:
    Scene* myScene;
    MainWindow* myMainWindow;
    
};

#endif // SUBSCENE_H

#ifndef LOADTHREAD_H
#define LOADTHREAD_H

#include <QThread>
#include <QXmlStreamReader>
class Scene;
class MainWindow;

class LoadThread : public QThread
{
    Q_OBJECT
public:
    explicit LoadThread(QObject *parent = 0);
    void setScene(Scene* myScene);
    void setFileName(QString fileName);
    void setXML(QXmlStreamReader* xml);
signals:

public slots:
protected:
    void run();
    Scene* myScene;
    QXmlStreamReader* myXml;
    QString myFileName;
    MainWindow* myMainWindow;
};

#endif // LOADTHREAD_H

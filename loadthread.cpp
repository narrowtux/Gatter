#include "loadthread.h"
#include "scene.h"
#include "mainwindow.h"

LoadThread::LoadThread(QObject *parent) :
    QThread(parent)
{
    myXml=0;
    myScene=0;
    myMainWindow=0;
    myFileName="";
}

void LoadThread::setFileName(QString fileName){
    myFileName=fileName;
}

void LoadThread::setScene(Scene *scene){
    myScene=scene;
    myMainWindow=scene->mainWindow();
}

void LoadThread::setXML(QCoreXmlStreamReader *xml){
    myXml=xml;
}

void LoadThread::run(){
    exec();
    if(myFileName==""&&myXml==0){
	return;
    }
    if(myScene==0){
	return;
    }
    QXmlStreamReader* xml;
    if(myFileName==""){
	xml=myXml;
    } else {
	xml=new QXmlStreamReader;
    }
}

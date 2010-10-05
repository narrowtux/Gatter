#include "subscene.h"
#include <QtGui>

SubScene::SubScene(QObject *parent) :
    Element(parent)
{
    myScene=new Scene;
    myMainWindow=new MainWindow(0,myScene);
    tr("SubScene");
    connect(myScene,SIGNAL(elementAddedOrRemoved()),this,SLOT(updateConnections()));
    height=50;
    width=50;
    myType="subscene";
    fileName="";
}

void SubScene::createFormBefore(){
    QPushButton* push=new QPushButton(tr("Select File"));
    additionalWidgets<<push;
    layout->addRow(push);
    connect(push,SIGNAL(clicked()),this,SLOT(selectFile()));
}

void SubScene::updateConnections(){
    sceneInputs.clear();
    sceneOutputs.clear();
    foreach(Element* e, myScene->elements){
	if(e->isInput()){
	    sceneInputs<<e;
	}
	if(e->isOutput()){
	    sceneOutputs<<e;
	    connect(e,SIGNAL(outputChanged(bool)),this,SLOT(recalculate()));
	}
    }
    setInputs(sceneInputs.count());
    setOutputs(sceneOutputs.count());
    setMinMaxInputsOutputs(sceneInputs.count(),sceneInputs.count(),sceneOutputs.count(),sceneOutputs.count());
}

void SubScene::recalculate(){
    int i=0;
    foreach(Element*e, sceneInputs){
	e->setInput(myInputs[i]->value());
	i++;
    }
    i=0;
    foreach(Element*e, sceneOutputs){
	myOutputs[i]->setValue(e->value());
	i++;
    }
}

void SubScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
    myMainWindow->show();
}

QRectF SubScene::boundingRect() const{
    return QRectF(0,0,width,height);
}

void SubScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter,option,widget);
    painter->drawText(boundingRect(),tr("Subccene"));
}

void SubScene::setPrivateXml(QCoreXmlStreamWriter *xml){
    xml->writeAttribute("src",fileName);
    if(fileName==""){
	myScene->save("",xml);
    }
    else{
	myScene->save(fileName);
    }
}

void SubScene::readPrivateXml(QCoreXmlStreamReader *xml){
    fileName=xml->attributes().value("src").toString();
    if(fileName=="")
	myScene->load("",xml);
    else
	loadFromFile(fileName);
    updateConnections();
}

void SubScene::loadFromFile(QString file){
    myScene->load(file);
    fileName=file;
    myMainWindow->setCurrentFile(fileName);
}

void SubScene::selectFile(){
    QSettings settings;
    loadFromFile(QFileDialog::getOpenFileName(0,tr("Select File"),settings.value("lastOpenDir").toString()));
}

SubScene::~SubScene(){
    myMainWindow->close();
    myMainWindow->deleteLater();
}

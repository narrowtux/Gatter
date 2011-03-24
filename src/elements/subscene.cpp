#include "src/elements/subscene.h"
#include <QtGui>
#include "src/scene/graphicsview.h"
#include "src/subsceneinfo.h"

QList<SubScene*> SubScene::subScenes = QList<SubScene*>();

SubScene::SubScene(QGraphicsObject *parent, bool createMainWindow) :
    Element(parent)
{
	Q_UNUSED(createMainWindow);
    myScene=new Scene;
	myScene->setSubscene(this);
    myMainWindow=0;
    tr("SubScene");
    connect(myScene,SIGNAL(elementAddedOrRemoved()),this,SLOT(updateConnections()));
    height=50;
    width=50;
    myType="subscene";
    fileName="";
    mySubSceneInfo=0;
	mySaveButton = myCloseButton = 0;
	myButtonsProxy = 0;
	subScenes<<this;
	myChildSubScene = 0;
	myParentSubScene = 0;
	myAction = new QAction("subscene",this);
	connect(myAction, SIGNAL(triggered()), this, SLOT(returnTo()));
}

bool SubScene::createFormBefore(){
    //QPushButton* push=new QPushButton(tr("Select File"));
    //additionalWidgets<<push;
    //layout->addRow(push);
    //connect(push,SIGNAL(clicked()),this,SLOT(selectFile()));
    return false;
}

void SubScene::updateConnections(){
    sceneInputs.clear();
    sceneOutputs.clear();
    inValues.clear();
    outValues.clear();
    QMultiMap<qreal,Element*> inputs, outputs;
    QList<qreal> inputP, outputP;
    foreach(Element* e, myScene->myElements){
		if(e->isInput()){
			inputs.insert(e->scenePos().y(),e);
			inputP<<e->scenePos().y();
		}
		if(e->isOutput()){
			outputs.insert(e->scenePos().y(),e);
			connect(e,SIGNAL(outputChanged(bool)),this,SLOT(recalcOutputs()),Qt::UniqueConnection);
			outputP<<e->scenePos().y();
		}
		if(e->isInput()||e->isOutput()){
			connect(e,SIGNAL(moved()),this,SLOT(updateConnections()),Qt::UniqueConnection);
		}
    }
    qSort(inputP);
    qSort(outputP);
    foreach(qreal v, inputP){
		sceneInputs<<inputs.value(v);
    }
    
    foreach(qreal v, outputP){
		sceneOutputs<<outputs.value(v);
		outValues<<outputs.value(v)->value();
    }
	
    setInputs(sceneInputs.count());
    setOutputs(sceneOutputs.count());
    setMinMaxInputsOutputs(sceneInputs.count(),sceneInputs.count(),sceneOutputs.count(),sceneOutputs.count());
    foreach(Connection*c, myInputs){
		inValues<<c->value();
    }
    for(int i=0;i<sceneInputs.count();i++){
		myInputs[i]->setName(sceneInputs[i]->name());
    }
    for(int i=0;i<sceneOutputs.count();i++){
		myOutputs[i]->setName(sceneOutputs[i]->name());
    }
	
    recalculate();
}

/*!
  Called when an input has changed. Won't update the outputs anymore!
  */

void SubScene::recalculate(){
    for(int i = 0; i<myInputs.count();i++){
		if(myInputs[i]->value()!=inValues[i]){
			sceneInputs[i]->setInput(myInputs[i]->value());
			inValues[i]=myInputs[i]->value();
		}
    }
}

void SubScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event){
	Q_UNUSED(event)
	//    if(myMainWindow==0){
	//		myMainWindow=new MainWindow(0, myScene);
	//		myMainWindow->setCurrentFile(fileName);
	//		myScene->setMainWindow(myMainWindow);
	//    }
	//    myMainWindow->show();
	
	myView = static_cast<GraphicsView *>(scene()->views().at(0));
	myContainerScene = static_cast<Scene *>(scene());
	setSelected(false);
	myView->setScene(myScene, true, this);
	if(myView->mainWindow()->openSubScene()==0){
		myView->mainWindow()->setOpenSubScene(this); //Tell MainWindow that this is the first open SubScene
	}
	myOpen = true;
}

QRectF SubScene::boundingRect() const{
    return QRectF(0,0,width,height);
}

void SubScene::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Element::paint(painter,option,widget);
    QString text=tr("Subscene");
    if(mySubSceneInfo!=0){
		text=mySubSceneInfo->name();
    }
    QFont font=painter->font();
    int fontsize=12;
    QRectF needs=painter->boundingRect(boundingRect().adjusted(3,3,-3,-3), text, QTextOption(Qt::AlignCenter));
    while (!boundingRect().adjusted(3,3,-3,-3).contains(needs)){
		fontsize--;
		font.setPixelSize(fontsize);
		painter->setFont(font);
		needs=painter->boundingRect(boundingRect().adjusted(3,3,-3,-3),text,QTextOption(Qt::AlignCenter));
		if(fontsize==8)break;
    }
    painter->drawText(boundingRect().adjusted(3,3,-3,-3),text,QTextOption(Qt::AlignCenter));
}

void SubScene::setPrivateXml(QXmlStreamWriter *xml){
    xml->writeAttribute("src",fileName);
    QString name;
    if(mySubSceneInfo!=0){
		xml->writeAttribute("name",name);
    }
    if(fileName==""){
		myScene->save("",xml);
    }
    else{
		myScene->save(fileName);
    }
}

void SubScene::readPrivateXml(QXmlStreamReader *xml){
    mySubSceneInfo=new SubSceneInfo;
    mySubSceneInfo->setFileName(fileName);
    fileName=xml->attributes().value("src").toString();
    QString name=xml->attributes().value("name").toString();
    if(name==""){
		name=tr("Subscene");
    }
    if(fileName=="")
		myScene->load("",xml);
    else
		loadFromFile(fileName);
    mySubSceneInfo->setName(name);
	myAction->setText(tr("Subscene: %0").arg(name));
    updateConnections();
}

void SubScene::loadFromFile(QString file, bool setAllAttributes){
    qDebug()<<"Loading SubScene from file:"<<file;
    if(QFile::exists(file))
		myScene->load(file,0,setAllAttributes);
    fileName=file;
    if(myMainWindow!=0)
		myMainWindow->setCurrentFile(fileName);
    updateConnections();
}

void SubScene::selectFile(){
    QSettings settings;
    loadFromFile(QFileDialog::getOpenFileName(0,tr("Select File"),settings.value("lastOpenDir").toString()));
}

SubScene::~SubScene(){
    myScene->deleteLater();
	subScenes.removeAll(this);
}

void SubScene::setFileName(QString fn){
    fileName=fn;
    if(myMainWindow!=0)
		myMainWindow->setCurrentFile(fileName);
}

void SubScene::setInfo(SubSceneInfo *info){
    mySubSceneInfo=info;
    update();
}

void SubScene::setMainWindow(MainWindow *m){
    myMainWindow=m;
}

void SubScene::loadEvent()
{
	updateConnections();
}

void SubScene::close()
{
	if(myOpen){
		if(myChildSubScene){
			if(myChildSubScene->isOpen()){
				myChildSubScene->close();
			}
		}
		if(myScene){
			myView->setScene(myContainerScene,true,this);
		}
		myView->mainWindow()->breadCumbBar()->removeAction(myAction);
		if(myView->mainWindow()->openSubScene()==this){
			myView->mainWindow()->setOpenSubScene(0); //Only set null if the last subscene is closed
		}
		myOpen = false;
		save();
	}
}

void SubScene::save()
{
	myScene->save(fileName);
	foreach(SubScene*i, subScenes)
	{
		if(i->fileName==fileName&&i!=this)
		{
			i->reload();
		}
	}
}

void SubScene::reload()
{
	myScene->clear();
	myScene->load(fileName);
	updateConnections();
}

void SubScene::setAction(QAction *action)
{
	myAction = action;
	connect(myAction, SIGNAL(triggered()), this, SLOT(returnTo()));
}

QAction * SubScene::action()
{
	return myAction;
}

void SubScene::returnTo()
{
	if(myChildSubScene){
		if(myChildSubScene->isOpen()){
			myChildSubScene->close();
		}
	}
}


void SubScene::setChildSubScene(SubScene *subscene)
{
	myChildSubScene = subscene;
	if(myChildSubScene)
		myChildSubScene->setParentSubScene(this);
}

void SubScene::setParentSubScene(SubScene *subscene)
{
	if(myParentSubScene!=subscene){
		myParentSubScene = subscene;
		if(myParentSubScene)
			myParentSubScene->setChildSubScene(this);
	}
}

Scene * SubScene::containingScene()
{
	return myScene;
}

SubScene * SubScene::childSubScene()
{
	return myChildSubScene;
}

SubScene * SubScene::parentSubScene()
{
	return myParentSubScene;
}

bool SubScene::isOpen()
{
	return myOpen;
}

/*!
  Called whenever an output on the scene changes.
  The separation of recalculate() and recalcOutputs() was done to prevent
  infinite loops.
  */

void SubScene::recalcOutputs()
{
	for(int i = 0; i<myOutputs.count();i++){
		if(sceneOutputs[i]->value()!=outValues[i]){
			myOutputs[i]->setValue(sceneOutputs[i]->value());
			outValues[i]=sceneOutputs[i]->value();
		}
	}
}

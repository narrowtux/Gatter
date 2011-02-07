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
}

bool SubScene::createFormBefore(){
    QPushButton* push=new QPushButton(tr("Select File"));
    additionalWidgets<<push;
    layout->addRow(push);
    connect(push,SIGNAL(clicked()),this,SLOT(selectFile()));
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
			connect(e,SIGNAL(outputChanged(bool)),this,SLOT(recalculate()),Qt::UniqueConnection);
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

void SubScene::recalculate(){
    int i=0;
    foreach(Element*e, sceneInputs){
		if(1||myInputs[i]->value()!=inValues[i]){
			e->setInput(myInputs[i]->value());
			inValues[i]=myInputs[i]->value();
		}
		i++;
    }
    i=0;
    foreach(Element*e, sceneOutputs){
		if(1||e->value()!=outValues[i]){
			myOutputs[i]->setValue(e->value());
			outValues[i]=e->value();
		}
		i++;
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
	
	GraphicsView *view = static_cast<GraphicsView *>(scene()->views().at(0));
	if(mySaveButton==0){
		mySaveButton = new QPushButton(tr("Save Subscene"));
		myCloseButton = new QPushButton(tr("Close"));
		myButtonsProxy = new QGraphicsProxyWidget;
		QWidget *centralWidget = new QWidget;
		QHBoxLayout *layout = new QHBoxLayout;
		centralWidget->setLayout(layout);
		layout->addWidget(mySaveButton);
		layout->addWidget(myCloseButton);
		myButtonsProxy->setWidget(centralWidget);
		QPointF pos;
		pos = view->mapToScene(0,0);
		myButtonsProxy->setPos(pos);
		myScene->addItem(myButtonsProxy);
		connect(myCloseButton, SIGNAL(clicked()), this, SLOT(close()));
		connect(mySaveButton, SIGNAL(clicked()), this, SLOT(save()));
	}
	myContainerScene = static_cast<Scene *>(scene());
	view->setScene(myScene, true);
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
		if(myScene)
			myScene->views().at(0)->setScene(myContainerScene);
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
}

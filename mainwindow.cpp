

//INCLUDES

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "gatter.h"
#include "switch.h"
#include "lamp.h"
#include "button.h"
#include "clock.h"
#include <qxmlstream.h>
#include <QSettings>
#include "subscene.h"
#include "subscenechoosedialog.h"
#include "delay.h"
#include <qxmlstream.h>
#include "flipflop.h"
#include "hexoutput.h"
#include "undoactions.h"


//DEFINES

QList<MainWindow*> MainWindow::mainWindows;
int MainWindow::unnamedIndex=0;
QList<QAction*> MainWindow::windowActions;
bool MainWindow::argvFileAlreadyOpened=false;
ElementCatalog* MainWindow::elementCatalog=0;


//CONSTRUCTORS

MainWindow::MainWindow(QWidget *parent, Scene *scene) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
#ifndef QT_ARCH_MACOSX
    ui->actionDelete->setShortcut(QKeySequence(Qt::Key_Delete));
#endif
    if(scene!=0){
		myScene=scene;
		mySubScene=true;
		myShouldBeSaved=false;
    }else{
		myScene=new Scene(this);
		mySubScene=false;
		myShouldBeSaved=true;
    }
    
    QAction *separatorAction;
    myUndoStack=new QUndoStack;
    separatorAction=new QAction(this);
    separatorAction->setSeparator(true);
    QAction *undo, *redo;
    undo=myUndoStack->createUndoAction(this,tr("Undo"));
    undo->setShortcut(QKeySequence("Ctrl+Z"));
    redo=myUndoStack->createRedoAction(this,tr("Redo"));
    redo->setShortcut(QKeySequence("Ctrl+Y"));
    ui->menuEdit->insertAction(ui->actionCut,undo);
    ui->menuEdit->insertAction(ui->actionCut,redo);
    ui->menuEdit->insertAction(ui->actionCut,separatorAction);
    
    myUndoView=new QUndoView(myUndoStack);
    ui->dockUndoView->widget()->setLayout(new QHBoxLayout);
    ui->dockUndoView->widget()->layout()->addWidget(myUndoView);
    
    ui->menuWindow->addAction( ui->dockUndoView->toggleViewAction());
    ui->menuWindow->addAction(ui->dockElementCatalog->toggleViewAction());
    
    
    ui->elementCatalog->setAcceptDrops(true);
    
    subSceneChooseDialog=new SubSceneChooseDialog;
    myScene->setMainWindow(this);
    ui->graphicsView->setScene(myScene);
    setCurrentFile("");
    mainWindows<<this;
    myAction=new QAction(windowTitle(),this);
    myAction->setCheckable(true);
    QAction* action=ui->dockInspector->toggleViewAction();
    action->setIcon(QIcon(":/icons/toolbar/inspector.png"));
    action->setShortcut(QKeySequence("Ctrl+Alt+I"));
    ui->menuWindow->addAction(action);
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(action);
    ui->dockInspector->close();
    ui->menuWindow->addAction(ui->dockUTDiagram->toggleViewAction());
    ui->menuWindow->addSeparator();
    windowActions<<myAction;
    connect(myAction,SIGNAL(triggered()),this,SLOT(raise()));
    connect(myAction,SIGNAL(triggered()),this,SLOT(setFocus()));
    foreach(MainWindow* m, mainWindows){
		m->updateActions();
    }
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionSave_As,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(myScene,SIGNAL(modified()),this,SLOT(documentWasModified()));
    connect(myScene,SIGNAL(elementMoved(QList<Element*>,QList<QPointF>)),this,SLOT(elementMoved(QList<Element*>,QList<QPointF>)));
    //loadFile("/Users/tux/test.gtr");
    ui->dockUTDiagram->close();
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
    
    myZoomOut=new QToolButton;
    myZoomOut->setText("-");
    myZoomOut->setToolTip(tr("Zoom Out"));
    ui->statusBar->addPermanentWidget(myZoomOut);
    connect(myZoomOut,SIGNAL(clicked()),this,SLOT(zoomOut()));
    
    myZoomSlider=new QSlider;
    myZoomSlider->setRange(0,100);
    myZoomSlider->setTickInterval(50);
    myZoomSlider->setValue(50);
    myZoomSlider->setOrientation(Qt::Horizontal);
    myZoomSlider->setTickPosition(QSlider::TicksBelow);
    ui->statusBar->addPermanentWidget(myZoomSlider);
    connect(myZoomSlider,SIGNAL(valueChanged(int)),this,SLOT(zoomTo(int)));
    
    myZoomIn=new QToolButton;
    myZoomIn->setText("+");
    myZoomIn->setToolTip(tr("Zoom In"));
    ui->statusBar->addPermanentWidget(myZoomIn);
    connect(myZoomIn,SIGNAL(clicked()),this,SLOT(zoomIn()));
    
    scale=1.0;
    separatorAction=new QAction(this);
    separatorAction->setSeparator(true);
    QList<QAction*> actions;
    actions<<ui->actionCut
			<<ui->actionCopy
			<<ui->actionPaste
			<<ui->actionDelete
			<<separatorAction
			<<ui->actionSelectAll;
    ui->graphicsView->addActions(actions);
    
    
    if(!argvFileAlreadyOpened){
		QStringList arguments=QApplication::arguments();
		if(arguments.count()>=2){
			QString file=arguments.at(1);
			if(QFileInfo(file).exists()){
				loadFile(file);
			}
		}
		argvFileAlreadyOpened=true;
    }
    QTimer* timer=new QTimer;
	timer->setSingleShot(true);
	timer->start(0);
	connect(timer,SIGNAL(timeout()),this,SLOT(readSettings()));
	readSettings();
}

MainWindow::~MainWindow()
{
    mainWindows.removeAll(this);
    delete ui;
}


//EVENTS

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    case QEvent::FileOpen:
		QFileOpenEvent*event=(QFileOpenEvent*)e;
		qDebug()<<event->file();
		break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
		writeSettings();
		event->accept();
		if(!isVisible()){
			windowActions.removeAll(myAction);
			myAction->deleteLater();
			updateActions();
		}
		if(!mySubScene){
			mainWindows.removeAll(this);
			delete myScene;
		}
    } else {
		event->ignore();
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateSceneRect();
}


//APPLICATION METHODS

MainWindow* MainWindow::newFile()
{
    MainWindow* m=new MainWindow(0);
    m->ui->elementCatalog->setModel(elementCatalog);
    m->show();
    return m;
}

void MainWindow::saveFileTo(QString fileName){
    save();
}

void MainWindow::loadFileFrom(QString fileName){
    open();
}

void MainWindow::open()
{
    QSettings settings;
    QString lastOpenDir=settings.value("lastOpenDir",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString();
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setWindowTitle(tr("Select File"));
    dialog.setDirectory(lastOpenDir);
    QStringList filters;
    filters<<tr("Gatter Files(*.gtr)")
			<<tr("All Files(*)");
    dialog.setFilters(filters);
    dialog.setLabelText(QFileDialog::LookIn, tr("Look In"));
    dialog.setLabelText(QFileDialog::FileName, tr("Name of File"));
    dialog.setLabelText(QFileDialog::FileType, tr("Files of Type"));
    dialog.setLabelText(QFileDialog::Accept, tr("Open"));
    dialog.setLabelText(QFileDialog::Reject, tr("Cancel"));
    dialog.exec();
    
    if (!dialog.selectedFiles().count()==0){
		foreach(QString fileName, dialog.selectedFiles()){
			settings.setValue("lastOpenDir",QFileInfo(fileName).absoluteDir().path());
			MainWindow*m;
			if(!myScene->isBlank())
				m=newFile();
			else
				m=this;
			m->loadFile(fileName);
		}
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
		return saveAs();
    } else {
		return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QSettings settings;
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setDefaultSuffix("gtr");
    dialog.setDirectory(settings.value("lastOpenDir",QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation)).toString());
    dialog.setWindowTitle(tr("Save File As"));
    dialog.exec();
    QString fileName;
    if (dialog.selectedFiles().count()==0)
		return false;
    fileName=dialog.selectedFiles().at(0);
    settings.setValue("lastOpenDir",QFileInfo(fileName).absoluteDir().path());
    return saveFile(fileName);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Gatter"),
					   tr("Gatter is a simulation for digital circuits."));
}

void MainWindow::documentWasModified()
{
    setWindowModified(true);
}

void MainWindow::readSettings()
{
    QSettings settings;
    restoreState(settings.value("state").toByteArray());
    restoreGeometry(settings.value("geometry").toByteArray());
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("state",saveState());
    settings.setValue("geometry",saveGeometry());
}

bool MainWindow::maybeSave()
{
    if (isWindowModified()&&myShouldBeSaved) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Gatter"),
								   tr("The document has been modified.\n"
									  "Do you want to save your changes?"),
								   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    //QtConcurrent::run(myScene,&Scene::load,QString(fileName),0,true);
    myScene->load(fileName);
    setCurrentFile(fileName);
}

bool MainWindow::saveFile(const QString &fileName)
{
    myScene->save(fileName);
    setCurrentFile(fileName);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);
    if(fileName!="")
		myShouldBeSaved=true;
    QString shownName = curFile;
    if (curFile.isEmpty())
		shownName = tr("untitled")+ QString().setNum(++unnamedIndex) +".gtr";
    setWindowFilePath(shownName);
    if(mySubScene){
		if(myShouldBeSaved){
			setWindowTitle(strippedName(shownName)+" "+tr("Subscene")+" - "+tr("Gatter"));
		} else {
			setWindowTitle(tr("Subscene")+" - "+tr("Gatter"));
		}
    } else {
		setWindowTitle(strippedName(shownName)+" - "+tr("Gatter"));
    }
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


//UPDATE METHODS

void MainWindow::updateActions()
{
    foreach(QAction* a,windowActions){
		ui->menuWindow->removeAction(a);
		if(hasFocus()){
			myAction->setChecked(true);
		}
    }
    ui->menuWindow->addActions(windowActions);
}

void MainWindow::updateSceneRect()
{
    if(ui->graphicsView->rect().contains(myScene->sceneRect().toRect())){
		//myScene->setSceneRect(ui->graphicsView->rect());
    }
}


//GETTERS

QFormLayout* MainWindow::getFormLayout(){
    return ui->selectionOptions;
}

Scene* MainWindow::scene(){
    return myScene;
}


//SLOTS

void MainWindow::zoomIn()
{
    int v=myZoomSlider->value()/10*10+10;
    myZoomSlider->setValue(v);
}

void MainWindow::zoomOut()
{
    int v=myZoomSlider->value()/10*10-10;
    myZoomSlider->setValue(v);
}

void MainWindow::zoomTo(int v){
    qreal value=v/100.0;
    if(value>0.45&&value<0.55&&value!=0.5){
		myZoomSlider->setValue(50);
		return;
    }
    scale=0.25*qPow(16.0,value);
    ui->graphicsView->setScale(scale);
}

void MainWindow::elementMoved(QList<Element *> e, QList<QPointF> oldPos){
    myUndoStack->push(new MoveElement(e,oldPos,this));
}

void MainWindow::initElementCatalog(){
    elementCatalog=new ElementCatalog(QApplication::instance());
	elementCatalog->load(":/data/standartCatalog.bin");
    ui->elementCatalog->setModel(elementCatalog);
}

//Qt Designer SLOTS

// Insert Actions

void MainWindow::on_actionInsertAND_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::AND);
    myUndoStack->push(new AddElement(g,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertOR_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::OR);
    myUndoStack->push(new AddElement(g,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertXOR_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::XOR);
    myUndoStack->push(new AddElement(g,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertNOT_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::NOT);
    myUndoStack->push(new AddElement(g,myScene->lastMousePos,this));
}

void MainWindow::on_actionMultiplexer_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::DUPLICATOR);
    myUndoStack->push(new AddElement(g,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertSwitch_triggered()
{
    myUndoStack->push(new AddElement(new Switch,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertLamp_triggered()
{
    myUndoStack->push(new AddElement(new Lamp,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertButton_triggered()
{
    myUndoStack->push(new AddElement(new Button,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertClock_triggered()
{
    myUndoStack->push(new AddElement(new Clock,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertSubscene_triggered()
{
    SubScene* scene=subSceneChooseDialog->getSubScene();
    if(scene!=0){
		myUndoStack->push(new AddElement(scene,myScene->lastMousePos,this));
    }
}

void MainWindow::on_actionInsertDelay_triggered()
{
    myUndoStack->push(new AddElement(new Delay,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertFlipflop_triggered()
{
    myUndoStack->push(new AddElement(new FlipFlop,myScene->lastMousePos,this));
}

void MainWindow::on_actionInsertHexOutput_triggered()
{
    myUndoStack->push(new AddElement(new HexOutput,myScene->lastMousePos,this));
}


//Other Actions

void MainWindow::on_actionDelete_triggered()
{
    foreach(QGraphicsItem*i,myScene->selectedItems()){
		if(myScene->isElement(i)){
			myUndoStack->push(new RemoveElement((Element*)i,this));
		} else {
			myScene->removeItem(i);
		}
    }
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::on_actionSelectAll_triggered()
{
    QPainterPath all;
    all.addRect(myScene->itemsBoundingRect());
    myScene->setSelectionArea(all);
}

void MainWindow::on_actionCopy_triggered()
{
    QList<Element*> selectedItems;
    foreach(QGraphicsItem* i, myScene->selectedItems()){
		if(myScene->isElement(i)){
			selectedItems<<(Element*)i;
		}
    }
    if(selectedItems.isEmpty()){
		return;
    }
    QClipboard* clipboard=QApplication::clipboard();
    QXmlStreamWriter* xml=new QXmlStreamWriter;
    QByteArray array;
    QBuffer buffer(&array);
    buffer.open(QIODevice::WriteOnly);
    xml->setDevice(&buffer);
    xml->writeStartDocument();
    myScene->save("",xml,selectedItems);
    xml->writeEndDocument();
    buffer.close();
    QMimeData* data=new QMimeData;
    data->setData("text/gatterxml",array);
    clipboard->setMimeData(data);
    delete xml;
}

void MainWindow::on_actionPaste_triggered()
{
    QClipboard* clipboard=QApplication::clipboard();
    if(!clipboard->mimeData()->hasFormat("text/gatterxml")){
		return;
    }
    myScene->paste(clipboard->mimeData(),myScene->lastMousePos);
}

void MainWindow::on_actionCut_triggered()
{
    on_actionCopy_triggered();
    on_actionDelete_triggered();
}

void MainWindow::on_actionRotate_triggered()
{
    foreach(QGraphicsItem* i, myScene->selectedItems()){
		qreal rotation=i->data(ElementRotation).toReal();
		rotation+=90;
		if(rotation>=360){
			rotation-=360;
		}
		i->setRotation(rotation);
		qDebug()<<i->rotation();
		i->setData(ElementRotation,rotation);
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    settingsDialog=new SettingsDialog(this);
    settingsDialog->show();
}

/*
 
void MainWindow::on_actionDistribute_Horizontally_triggered()
{
    if(myScene->selectedItems().count()>1){
	int left=INT_MAX, right=INT_MIN, totalWidth=0;
	foreach(QGraphicsItem*i, myScene->selectedItems()){
	    left=qMin(left,(int)i->x());
	    right=qMax(right,(int)(i->x()+i->boundingRect().width()));
	    totalWidth+=i->boundingRect().width();
	}
	int space=abs(right-left-totalWidth)/myScene->selectedItems().count();
	qDebug()<<"Left:"<<left<<"Right:"<<right<<"Space:"<<space;
	int pos=left;
	foreach(QGraphicsItem*i, myScene->selectedItems()){
	    i->setX(pos+0.5);
	    pos+=i->boundingRect().width()+space;
	}
    }
}
*/

void MainWindow::on_actionLayoutLeft_triggered()
{
    
}

void MainWindow::on_actionLayoutCenter_triggered()
{
    if(myScene->selectedItems().count()>1){
		int center=0;
		foreach(QGraphicsItem* i, myScene->selectedItems()){
			center+=i->x();
		}
		center/=myScene->selectedItems().count();
		foreach(QGraphicsItem*i, myScene->selectedItems()){
			i->setX(center+0.5);
		}
    }
}

void MainWindow::on_actionLayoutMiddle_triggered()
{
    if(myScene->selectedItems().count()>1){
		int center=0;
		foreach(QGraphicsItem* i, myScene->selectedItems()){
			center+=i->y();
		}
		center/=myScene->selectedItems().count();
		foreach(QGraphicsItem*i, myScene->selectedItems()){
			i->setY(center+0.5);
		}
    }
}

void MainWindow::on_toolRemoveTemplate_clicked()
{
    elementCatalog->removeRows(ui->elementCatalog->selectionModel()->currentIndex().row(),1,QModelIndex());
}

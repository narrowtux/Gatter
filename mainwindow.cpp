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
QList<MainWindow*> MainWindow::mainWindows;
int MainWindow::unnamedIndex=0;
QList<QAction*> MainWindow::windowActions;


MainWindow::MainWindow(QWidget *parent, Scene *scene) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if(scene!=0){
	myScene=scene;
	mySubScene=true;
	myShouldBeSaved=false;
    }else{
	myScene=new Scene(this);
	mySubScene=false;
	myShouldBeSaved=true;
    }
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
}

MainWindow::~MainWindow()
{
    mainWindows.removeAll(this);
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

MainWindow* MainWindow::newFile()
{
    MainWindow* m=new MainWindow(0);
    m->show();
    return m;
}

void MainWindow::on_actionClose_triggered()
{
    close();
}

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

void MainWindow::on_actionInsertAND_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::AND);
    myScene->addElement(g);
}

void MainWindow::on_actionInsertOR_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::OR);
    myScene->addElement(g);
}

void MainWindow::on_actionInsertXOR_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::XOR);
    myScene->addElement(g);
}

void MainWindow::on_actionInsertNOT_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::NOT);
    myScene->addElement(g);
}

void MainWindow::on_actionMultiplexer_triggered()
{
    Gatter*g=new Gatter;
    g->setType(Gatter::DUPLICATOR);
    myScene->addElement(g);
}

void MainWindow::on_actionInsertSwitch_triggered()
{
    myScene->addElement(new Switch);
}

void MainWindow::on_actionInsertLamp_triggered()
{
    myScene->addElement(new Lamp);
}

void MainWindow::on_actionDelete_triggered()
{
    foreach(QGraphicsItem*i,myScene->selectedItems()){
	myScene->removeItem(i);
	delete i;
    }
}

void MainWindow::on_actionInsertButton_triggered()
{
    myScene->addElement(new Button);
}


QFormLayout* MainWindow::getFormLayout(){
    return ui->selectionOptions;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    updateSceneRect();
}

void MainWindow::updateSceneRect()
{
    if(ui->graphicsView->rect().contains(myScene->sceneRect().toRect())){
	//myScene->setSceneRect(ui->graphicsView->rect());
    }
}

void MainWindow::on_actionInsertClock_triggered()
{
    myScene->addElement(new Clock);
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
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
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

void MainWindow::on_actionInsertSubscene_triggered()
{
    SubScene* scene=subSceneChooseDialog->getSubScene();
    if(scene!=0){
	myScene->addElement(scene);
    }
}

void MainWindow::on_actionInsertDelay_triggered()
{
    myScene->addElement(new Delay);
}

void MainWindow::on_actionInsertFlipflop_triggered()
{
    myScene->addElement(new FlipFlop(0));
}

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
    QTransform tr;
    tr.scale(scale,scale);
    ui->graphicsView->setTransform(tr);
}

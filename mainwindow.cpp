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
QList<MainWindow*> MainWindow::mainWindows;
int MainWindow::unnamedIndex=0;
QList<QAction*> MainWindow::windowActions;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myScene=new Scene(this);
    myScene->setMainWindow(this);
    ui->graphicsView->setScene(myScene);

    setCurrentFile("");
    mainWindows<<this;
    myAction=new QAction(windowTitle(),this);
    myAction->setCheckable(true);
    ui->menuWindow->addAction(ui->dockInspector->toggleViewAction());
    ui->menuWindow->addAction(ui->dockUTDiagram->toggleViewAction());
    ui->menuWindow->addSeparator();
    windowActions<<myAction;
    connect(myAction,SIGNAL(triggered()),this,SLOT(raise()));
    connect(myAction,SIGNAL(triggered()),this,SLOT(setFocus()));
    foreach(MainWindow* m, mainWindows){
	m->updateActions();
    }
    ui->spinDelay->setValue(Gatter::delayMS);
    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(save()));
    connect(ui->actionSave_As,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(ui->actionAbout,SIGNAL(triggered()),this,SLOT(about()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(open()));
    connect(ui->actionNew,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(myScene,SIGNAL(modified()),this,SLOT(documentWasModified()));
    //myScene->setSceneRect(ui->graphicsView->rect());
    loadFile("/Users/tux/test.gtr");
}

MainWindow::~MainWindow()
{
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

void MainWindow::closeEvent(QCloseEvent *event){

    if (maybeSave()) {
	writeSettings();
	event->accept();
	windowActions.removeAll(myAction);
	myAction->deleteLater();
	updateActions();
	mainWindows.removeAll(this);
    } else {
	event->ignore();
    }
}

void MainWindow::on_actionInsertAND_triggered()
{
    Gatter*g=new Gatter(this);
    g->setType(Gatter::AND);
    myScene->addElement(g);
}

void MainWindow::on_actionInsertOR_triggered()
{
    Gatter*g=new Gatter(this);
    g->setType(Gatter::OR);
    myScene->addElement(g);
}

void MainWindow::on_actionInsertXOR_triggered()
{
    Gatter*g=new Gatter(this);
    g->setType(Gatter::XOR);
    myScene->addElement(g);
}

void MainWindow::on_actionInsertNOT_triggered()
{
    Gatter*g=new Gatter(this);
    g->setType(Gatter::NOT);
    myScene->addElement(g);
}

void MainWindow::on_actionMultiplexer_triggered()
{
    Gatter*g=new Gatter(this);
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

void MainWindow::on_spinDelay_valueChanged(int val)
{
    Gatter::delayMS=val;
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

void MainWindow::on_zoomSlider_valueChanged(int value)
{
   myScene->setScale(value/100.0);
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
    if (maybeSave()) {
	QString fileName = QFileDialog::getOpenFileName(this);
	if (!fileName.isEmpty()){
	    MainWindow*m=newFile();
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
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
	return false;

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
    if (isWindowModified()) {
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

    QString shownName = curFile;
    if (curFile.isEmpty())
	shownName = tr("untitled")+ QString().setNum(++unnamedIndex) +".gtr";
    setWindowFilePath(shownName);
    setWindowTitle(strippedName(shownName)+" - "+tr("Gatter"));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "gatter.h"
#include "switch.h"
#include "lamp.h"
#include "button.h"

QList<MainWindow*> MainWindow::mainWindows;
int MainWindow::unnamedIndex=0;
QList<QAction*> MainWindow::windowActions;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myScene=new Scene(this);
    ui->graphicsView->setScene(myScene);

    fileName=tr("unnamed ")+QString().setNum(++unnamedIndex);
    setWindowTitle(fileName);
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

void MainWindow::on_actionNew_triggered()
{
    MainWindow* m=new MainWindow(0);
	m->show();
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

void MainWindow::closeEvent(QCloseEvent *){
    windowActions.removeAll(myAction);
    myAction->deleteLater();
    updateActions();
    mainWindows.removeAll(this);
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

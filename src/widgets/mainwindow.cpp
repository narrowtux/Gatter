

//INCLUDES


#include "src/widgets/mainwindow.h"
#include "ui_mainwindow.h"
#include "src/elements/element.h"
#include "src/scene/scene.h"
#include "src/elements/gatter.h"
#include "src/elements/switch.h"
#include "src/elements/lamp.h"
#include "src/elements/button.h"
#include "src/elements/clock.h"
#include <qxmlstream.h>
#include <QSettings>
#include "src/elements/subscene.h"
#include "src/widgets/subscenechoosedialog.h"
#include "src/elements/delay.h"
#include <qxmlstream.h>
#include "src/elements/flipflop.h"
#include "src/elements/sevensegmentoutput.h"
#include "src/undoactions.h"
#include "src/elements/distributor.h"


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
    
	if(mainWindows.count()>0){
		ui->elementCatalog->setModel(elementCatalog);
		ui->elementCatalog->expandAll();
	}
	
	
#ifndef QT_ARCH_MACOSX
    ui->actionDelete->setShortcut(QKeySequence(Qt::Key_Delete));
#endif
	
	
    setScene(new Scene);
	myShouldBeSaved = true;
	mySubScene = false;
	
	QSettings settings;
	Element::rotationSteps=settings.value("rotationSteps",90).toReal();
    

	
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
   	QMenu * menu=new QMenu;
	ui->addUTRecordingButton->setMenu(menu);
	connect(menu, SIGNAL(aboutToShow()), this, SLOT(updateConnectionAddMenu()));
	connect(menu, SIGNAL(aboutToHide()), myScene, SLOT(clearHighlight()));
    //loadFile("/Users/tux/test.gtr");
    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	ui->graphicsView->setMainWindow(this);
	
	myZoomBox = new QSpinBox;
	myZoomBox->setRange(25,800);
	myZoomBox->setValue(100);
	myZoomBox->setSuffix(" %");
	myZoomBox->setAccelerated(true);
	myZoomBox->setSingleStep(10);
	connect(myZoomBox, SIGNAL(valueChanged(int)), this, SLOT(zoomTo(int)));
	ui->statusBar->addPermanentWidget(myZoomBox);
	
	
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
	action=new QAction(tr("Add label"),this);
	connect(action,SIGNAL(triggered()),this,SLOT(addLabel()));
	ui->toolAddTemplateFromFile->addAction(action);
	action=new QAction(tr("Add from file"),this);
	connect(action,SIGNAL(triggered()), this,SLOT(addFromFile()));
	ui->toolAddTemplateFromFile->addAction(action);
	action = new QAction(tr("Add Subscene"), this);
	connect(action, SIGNAL(triggered()), this, SLOT(addSubscene()));
	ui->toolAddTemplateFromFile->addAction(action);
	
	printDialog=0;
	
	myDiagram=new UTDiagram;
	ui->UTRecordLayout->addWidget(myDiagram);
	myDiagram->setRecordingStatus(false);
	
    QTimer* timer=new QTimer;
	timer->setSingleShot(true);
	timer->start(0);
	connect(timer,SIGNAL(timeout()),this,SLOT(readSettings()));
	
	readSettings();
	
	connect(ui->graphicsView, SIGNAL(scaleFactorChanged(int)), myZoomBox, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    mainWindows.removeAll(this);
	delete myDiagram;
    delete ui;
}


//EVENTS

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
	QFileOpenEvent*event;
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    case QEvent::FileOpen:
		event=(QFileOpenEvent*)e;
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
    MainWindow* m=new MainWindow;
    m->ui->elementCatalog->setModel(elementCatalog);
    m->show();
    return m;
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
	int width=ui->graphicsView->rect().width(), height=ui->graphicsView->rect().height();
	qreal scale=ui->graphicsView->scaleFactor();
	width/=scale;
	height/=scale;
	sceneFitRect->setRect(-width/2,-height/2,width,height);
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
    ui->graphicsView->setScale((qreal)v/100.0);
}

void MainWindow::elementMoved(QList<Element *> e, QList<QPointF> oldPos){
    myUndoStack->push(new MoveElement(e,oldPos,this));
}

void MainWindow::initElementCatalog(){
    elementCatalog=new ElementCatalog(QApplication::instance());
	//elementCatalog->load(":/data/standartCatalog.bin");
    ui->elementCatalog->setModel(elementCatalog);
	ui->elementCatalog->expandAll();
}

void MainWindow::addLabel(){
	QString name=QInputDialog::getText(this, tr("Name of Label"), tr("Name of Label"));
	if(name=="")
		return;
	elementCatalog->addItem(name, "");
}

void MainWindow::addFromFile(){
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
			QFile file(fileName);
			QString xml;
			QString name=QInputDialog::getText(this,tr("Name for File"),tr("Name for File"),QLineEdit::Normal,QFileInfo(fileName).baseName());
			if(name!=""){
				file.open(QIODevice::ReadOnly);
				xml=file.readAll();
				file.close();
				elementCatalog->addItem(name, xml);
			}
		}
    }
}

//Qt Designer SLOTS

// Insert Actions

void MainWindow::on_actionInsertSubscene_triggered()
{
    SubScene* scene=subSceneChooseDialog->getSubScene();
    if(scene!=0){
		myUndoStack->push(new AddElement(scene,myScene->lastMousePos,this));
    }
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
		rotation+=Element::rotationSteps;
		qreal aniRotation = rotation;
		if(rotation>=360){
			rotation-=360;
		}
		QGraphicsObject *obj = static_cast<QGraphicsObject *>(i);
		if(obj!=0){
			QPropertyAnimation *ani = new QPropertyAnimation(obj, "rotation", this);
			ani->setEndValue(aniRotation);
			ani->setDuration(200);
			ani->start(QAbstractAnimation::DeleteWhenStopped);
		} else {
			i->setRotation(rotation);
		}
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
    elementCatalog->removeRows(ui->elementCatalog->selectionModel()->currentIndex().row(),1,ui->elementCatalog->selectionModel()->currentIndex().parent());
}

void MainWindow::on_actionPrint_triggered()
{
	if(printDialog!=0)
		delete printDialog;
	QPrinter* printer=new QPrinter(QPrinter::HighResolution);
	printDialog=new QPrintDialog(printer,this);
    printDialog->open();
	connect(printDialog, SIGNAL(accepted()), this, SLOT(printDialogClosed()));
}

void MainWindow::printDialogClosed()
{
	QPrinter *printer=printDialog->printer();
	printer->setResolution(300);
	QPainter painter(printer);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);
	myScene->render(&painter);
}

void MainWindow::updateConnectionAddMenu()
{
    //Update Connections
	QList<Element*> elements=myScene->elementList();
	QList<Connection*> connections;
	foreach(Element* element, elements){
		connections<<element->inputs()<<element->outputs();
	}

	QMenu* menu=ui->addUTRecordingButton->menu();
	if(menu==0){
		menu=new QMenu;
		ui->addUTRecordingButton->setMenu(menu);
	}
	menu->clear();
	QList<QAction*> actions;
	foreach (Connection* connection, connections){
		QString name=connection->name();
		if(name==""){
			name=tr("(No name)");
		}
		name+=tr(" on %0").arg(connection->element()->metaObject()->className());
		if(connection->connectionType()==Input){
			name+=" ("+tr("Input")+")";
		} else {
			name+=" ("+tr("Output")+")";
		}
		QAction * action=new QAction(name, this);
		QVariant pointer=qVariantFromValue<void*>(connection);
		action->setProperty("connectionPointer", pointer);
		connect(action, SIGNAL(hovered()), this, SLOT(connectionAddMenuHover()));
		connect(action, SIGNAL(triggered()), this, SLOT(addUTConnectionRecording()));
		actions<<action;
	}
	menu->addActions(actions);
}

void MainWindow::connectionAddMenuHover()
{
	QAction * action = qobject_cast<QAction*>(sender());
	Connection* connection = static_cast<Connection*>(qVariantValue<void*>(action->property("connectionPointer")));
	myScene->highlight(connection);
}

void MainWindow::on_recordUTButton_toggled(bool checked)
{
    myDiagram->setRecordingStatus(checked);
}

void MainWindow::addUTConnectionRecording()
{
	QAction * action = qobject_cast<QAction*>(sender());
	Connection* connection = static_cast<Connection*>(qVariantValue<void*>(action->property("connectionPointer")));
	connect(connection, SIGNAL(changed(bool)), myDiagram, SLOT(changeSignal(bool)));
	connection->flushValue();
}


void MainWindow::on_autoScrollButton_clicked(bool checked)
{
    myDiagram->setAutoScrollStatus(checked);
}

void MainWindow::on_actionInsertByTypeName_triggered()
{
    QString typeName = QInputDialog::getText(this, tr("Enter Typename"), tr("Enter the Typename. For example: button"));
	if(typeName == ""){
		return;
	}
	Element * element = myScene->getElementFromTypeName(typeName);
	if(element == 0){
		QMessageBox::information(this, tr("Not found"), tr("The system has not found the type %0.").arg(typeName));
	} else {
		myScene->addElement(element);
	}
}

void MainWindow::addSubscene()
{
	QString nameOfSubscene = QInputDialog::getText(this, tr("Name of Subscene"), tr("Enter the Name of the Subscene"), QLineEdit::Normal, tr("My Subscene"));
	if(nameOfSubscene != ""){
		QFile catalogTemplate(":/data/subscene_catalog_template");
		catalogTemplate.open(QIODevice::ReadOnly);
		QString xmlTemplate = catalogTemplate.readAll();
		catalogTemplate.close();
		xmlTemplate = xmlTemplate.arg(nameOfSubscene);
		QDir myDirectory=QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
	    QString fn=nameOfSubscene.toLower().simplified();
	    fn=myDirectory.absolutePath()+"/"+fn+".gtr";
		xmlTemplate = xmlTemplate.arg(fn);
		QFile fileTemplate(":/data/subscene_file_template");
		fileTemplate.open(QIODevice::ReadOnly);
		QFile subceneFile(fn);
		if(subceneFile.open(QIODevice::WriteOnly))
		{
			subceneFile.write(fileTemplate.readAll());
			elementCatalog->addItem(nameOfSubscene, xmlTemplate);
		} else {
			QMessageBox::critical(this, tr("Error"), tr("Aborted because the Template file couldn't be written to your Subscene Directory. Try to choose an other name."));
		}
	}
}

void MainWindow::setScene(Scene *scene)
{
	myScene = scene;
	sceneFitRect=new QGraphicsRectItem;
	myScene->addItem(sceneFitRect);
	sceneFitRect->setBrush(Qt::NoBrush);
	sceneFitRect->setPen(Qt::NoPen);
    myScene->setMainWindow(this);
    ui->graphicsView->setScene(myScene); 
	connect(myScene,SIGNAL(modified()),this,SLOT(documentWasModified()));
    connect(myScene,SIGNAL(elementMoved(QList<Element*>,QList<QPointF>)),this,SLOT(elementMoved(QList<Element*>,QList<QPointF>)));

}



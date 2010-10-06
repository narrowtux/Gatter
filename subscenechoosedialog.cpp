#include "subscenechoosedialog.h"
#include "ui_subscenechoosedialog.h"
#include <QDesktopServices>
#include <qxmlstream.h>
SubSceneChooseDialog::SubSceneChooseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubSceneChooseDialog)
{
    ui->setupUi(this);
    myDirectory=QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
    if(!myDirectory.exists()){
	qDebug()<<"Creating directory"<<myDirectory<<": "<<myDirectory.mkpath(myDirectory.absolutePath());
    }
    updateExisting();
}

SubSceneChooseDialog::~SubSceneChooseDialog()
{
    saveExisting();
    delete ui;
}

SubScene* SubSceneChooseDialog::getSubScene(){
    exec();
    if(result()==Accepted){
	SubScene* scene=new SubScene;
	QString fileName;
	if(ui->radioExisting->isChecked()){
	    if(ui->listExistingScenes->selectedItems().count()!=0){
		QListWidgetItem*item=ui->listExistingScenes->selectedItems().at(0);
		fileName=item->data(QListWidgetItem::UserType+1).toString();
	    }
	    scene->loadFromFile(fileName);
	} else {
	    SubSceneInfo*info=new SubSceneInfo;
	    information<<info;
	    info->setName(ui->lineNameOfScene->text());
	    info->setProtected(false);
	    QString fn=info->name().toLower().simplified();
	    fn=myDirectory.absolutePath()+"/"+fn+".gtr";
	    info->setFileName(fn);
	    scene->setFileName(fn);
	    saveExisting();
	    updateExisting();
	}
	return scene;
    } else {
	return 0;
    }
}

void SubSceneChooseDialog::updateExisting(){
    ui->listExistingScenes->clear();
    information.clear();
    if(QFile::exists(myDirectory.absolutePath()+"/templatedatabase.xml")){
	QXmlStreamReader xml;
	QFile file(myDirectory.absolutePath()+"/templatedatabase.xml");
	file.open(QIODevice::ReadOnly);
	xml.setDevice(&file);
	while(!xml.atEnd()){
	    xml.readNext();
	    if(xml.name()=="template"&&!xml.isEndElement()){
		QXmlStreamAttributes attr=xml.attributes();
		SubSceneInfo* info=new SubSceneInfo;
		info->setFileName(myDirectory.absolutePath()+"/"+attr.value("file").toString());
		info->setName(attr.value("name").toString());
		info->setProtected(attr.value("protected").toString()=="true"?1:0);
		information<<info;
		QListWidgetItem* item=new QListWidgetItem;
		item->setText(info->name());
		//item->setIcon(*info->icon());
		item->setData(QListWidgetItem::UserType+1,QVariant(info->fileName()));
		ui->listExistingScenes->addItem(item);
	    }
	}
	file.close();
    }
}

void SubSceneChooseDialog::saveExisting(){
    QXmlStreamWriter xml;
    QFile file(myDirectory.absolutePath()+"/templatedatabase.xml");
    file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    xml.setDevice(&file);
    xml.writeStartDocument();
    xml.writeStartElement("database");
    foreach(SubSceneInfo*info, information){
	xml.writeStartElement("template");
	xml.writeAttribute("name",info->name());
	xml.writeAttribute("file",QFileInfo(info->fileName()).fileName());
	xml.writeAttribute("protected",info->isProtected()?"true":"false");
	xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
    file.close();
}

void SubSceneChooseDialog::on_toolAddSubScene_clicked()
{
    QSettings settings;
    QString fn=QFileDialog::getOpenFileName(this, tr("Choose a File to import as Subscene"),settings.value("lastOpenDir").toString());
    QString newFile=myDirectory.absolutePath()+"/"+QFileInfo(fn).fileName();
    QFile::copy(fn, newFile);
    QString name= QInputDialog::getText(this, tr("Name of the Scene"), tr("Please enter a name for this scene"));
    SubSceneInfo* info=new SubSceneInfo;
    info->setName(name);
    info->setFileName(newFile);
    info->setProtected(false);
    information<<info;
    saveExisting();
    updateExisting();
}

void SubSceneChooseDialog::showEvent(QShowEvent *){
    saveExisting();
    updateExisting();
}

void SubSceneChooseDialog::on_toolRemoveSubScene_clicked()
{
    if(ui->listExistingScenes->selectedItems().count()!=0){
	QListWidgetItem* item=ui->listExistingScenes->selectedItems().at(0);
	QString fn=item->data(QListWidgetItem::UserType+1).toString();
	foreach(SubSceneInfo*info, information){
	    if(info->fileName()==fn&&!info->isProtected()){
		delete info;
		information.removeAll(info);
		QFile::remove(fn);
	    }
	}
	saveExisting();
	updateExisting();
    }
}

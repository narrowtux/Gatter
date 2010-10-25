#include "elementcatalog.h"
#include <QtGui>
ElementCatalog::ElementCatalog(QObject *parent) :
		QAbstractListModel(parent)
{
    setSupportedDragActions(Qt::CopyAction);
	load();
}

ElementCatalog::~ElementCatalog(){
	save();
}

void ElementCatalog::addData(QString name, QString value, int row){
    if(row==-1){
		row=myData.size();
    }
    beginInsertRows(QModelIndex(),myData.count()-1,myData.count()-1);
    myData.insert(row,QPair<QString,QString>(name, value));
    endInsertRows();
}

int ElementCatalog::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return myData.count();
}

QVariant ElementCatalog::data(const QModelIndex &index, int role) const{
    int r=index.row();
    if(role==Qt::DisplayRole){
		return myData.at(r).first;
    }
    return QVariant(QVariant::Invalid);
}

QMimeData* ElementCatalog::mimeData(const QModelIndexList &indexes) const{
    QMimeData* ret=new QMimeData;
    if(indexes.count()>=1){
		ret->setData("text/gatterxml",myData.at(indexes.at(0).row()).second.toLocal8Bit());
		ret->setText(myData.at(indexes.at(0).row()).first);
    }
    return ret;
}

Qt::ItemFlags ElementCatalog::flags(const QModelIndex &index) const{
    Qt::ItemFlags def=QAbstractItemModel::flags(index);
    if(index.isValid()){
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | def;
    } else {
		return Qt::ItemIsDropEnabled | def;
    }
}

bool ElementCatalog::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
    Q_UNUSED(column)
    Q_UNUSED(parent)
    if(action==Qt::IgnoreAction){
		return true;
    }
    if(data->hasFormat("text/gatterxml")){
		if(data->hasText()&&data->hasFormat("text/gatterxml")){
			QString name=data->text();
			QString xml=QString(data->data("text/gatterxml")).toLatin1();
			int index=0;
			QPair<QString,QString> pair;
			foreach(pair, myData){
				if(pair.first==name){
					break;
				}
				index++;
			}
			beginRemoveRows(QModelIndex(),index,index);
			myData.removeAt(index);
			endRemoveRows();
			if(index<row){
				row--;
			}
			beginInsertRows(QModelIndex(),row,row);
			addData(name, xml, row);
			endInsertRows();
			return true;
		} else {
			QString name=QInputDialog::getText(0,tr("Name"), tr("Please enter a name for this template"));
			if(name=="")
				return false;
			addData(name,QString(data->data("text/gatterxml")).toLatin1(),row);
			return true;
		}
    } else {
		return false;
    }
}

Qt::DropActions ElementCatalog::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList ElementCatalog::mimeTypes() const{
    return QStringList()<<"text/gatterxml";
}

void ElementCatalog::load(){
	QString myDirName=QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QDir dir(myDirName);
	if(!dir.exists()){
		if(!dir.mkpath(myDirName))
			return;
	}
	QString fileName=myDirName+"/elementCatalog.bin";
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QVariant vars;
	QDataStream ds(&file);
	ds>>vars;
	file.close();
	
	QList<QVariant> list=vars.toList();
	QList<QPair<QString, QString> > pairs;
	foreach(QVariant pair, list){
		QList<QString> stringList=pair.toStringList();
		pairs<<QPair<QString,QString>(stringList[0],stringList[1]);
	}
	beginInsertRows(QModelIndex(),0,pairs.count()-1);
	myData=pairs;
	endInsertRows();
}

void ElementCatalog::save(){
	QString myDirName=QDesktopServices::storageLocation(QDesktopServices::DataLocation);
	QDir dir(myDirName);
	if(!dir.exists()){
		if(!dir.mkpath(myDirName))
			return;
	}
	QString fileName=myDirName+"/elementCatalog.bin";
	QFile file(fileName);
	file.open(QIODevice::WriteOnly);
	QVariant vars;
	QList<QVariant> list;
	QPair<QString, QString> pair;
	foreach(pair, myData){
		list<<QVariant(QList<QString>()<<pair.first<<pair.second);
	}
	vars.setValue(list);
	QDataStream ds(&file);
	ds<<vars;
	file.close();
}

bool ElementCatalog::setData(const QModelIndex &index, const QVariant &value, int role){
	if(role==Qt::EditRole&&index.row()<myData.count()&&value.toString()!=""){
		myData[index.row()].first=value.toString();
		return true;
	}
	return false;
}

bool ElementCatalog::removeRows(int row, int count, const QModelIndex &parent){
	beginRemoveRows(parent, row, row+count-1);
	for(int index=row;index<row+count;index++){
		myData.removeAt(index);
	}
	endRemoveRows();
}

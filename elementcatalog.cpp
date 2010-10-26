#include "elementcatalog.h"
#include <QtGui>

#ifndef USE_OLD_ELEMENTCATALOG

ElementCatalog::ElementCatalog(QObject *parent)
    : QAbstractItemModel(parent)
{
	load();
	setSupportedDragActions(Qt::CopyAction|Qt::MoveAction);
}

ElementCatalog::~ElementCatalog()
{
	save();
    delete rootItem;
}

int ElementCatalog::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<CatalogItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant ElementCatalog::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    CatalogItem *item = static_cast<CatalogItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags ElementCatalog::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
}

QVariant ElementCatalog::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex ElementCatalog::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    CatalogItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CatalogItem*>(parent.internalPointer());

    CatalogItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex ElementCatalog::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    CatalogItem *childItem = static_cast<CatalogItem*>(index.internalPointer());
    CatalogItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int ElementCatalog::rowCount(const QModelIndex &parent) const
{
    CatalogItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<CatalogItem*>(parent.internalPointer());

    return parentItem->childCount();
}

void ElementCatalog::setupModelData(const QStringList &lines, CatalogItem *parent)
{
    QList<CatalogItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QList<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            parents.last()->appendChild(new CatalogItem(columnData, parents.last()));
        }

        number++;
    }
}

QMimeData* ElementCatalog::mimeData(const QModelIndexList &indexes) const{
	if(indexes.count()>0){
		const QModelIndex index=indexes.first();
		CatalogItem *item = static_cast<CatalogItem*>(index.internalPointer());
		QMimeData* mimeData=new QMimeData;
		mimeData->setText(item->name());
		if(!item->isLabel())
			mimeData->setData("text/gatterxml",item->xml().toLocal8Bit());
		else
			mimeData->setData("bool/islabel", "");
		return mimeData;
	}
	return 0;
}

bool ElementCatalog::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
	if(!(data->hasFormat("text/gatterxml")||data->hasFormat("bool/islabel"))){
		return false;
	}
	CatalogItem*parentItem;
	if(parent.isValid())
		parentItem=static_cast<CatalogItem*>(parent.internalPointer());
	else
		parentItem=rootItem;
	if(action==Qt::CopyAction||action==Qt::MoveAction){
		QString xml=data->data("text/gatterxml");
		QString name;
		if(action==Qt::MoveAction){
			name=data->text();
		} else {
			name=QInputDialog::getText(0, tr("Enter name"), tr("Please enter the name for this Template"));
			if(name==""){
				return false;
			}
		}
		CatalogItem*item=new CatalogItem(QList<QVariant>()<<name<<xml<<false,parentItem);
		beginInsertRows(parent, row, row);
		parentItem->insertChild(row, item);
		endInsertRows();
		return true;
	}
}

Qt::DropActions ElementCatalog::supportedDropActions() const{
    return Qt::CopyAction | Qt::MoveAction;
}

QStringList ElementCatalog::mimeTypes() const{
    return QStringList()<<"text/gatterxml"<<"bool/islabel";
}

bool ElementCatalog::removeRows(int row, int count, const QModelIndex &parent){
	CatalogItem*parentItem;
	if(parent.isValid())
		parentItem=static_cast<CatalogItem*>(parent.internalPointer());
	else
		parentItem=rootItem;
	beginRemoveRows(parent,row,row+count-1);
	for(int i=row;i<row+count;i++){
		parentItem->removeChild(i);
	}
	endRemoveRows();
	return true;
}

void ElementCatalog::addItem(QString name, QString xml){
	beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
	rootItem->appendChild(new CatalogItem(QList<QVariant>()<<name<<xml<<false,rootItem));
	endInsertRows();
}

void ElementCatalog::load(QString fileName){
	if(fileName==""){
		QString myDirName=QDesktopServices::storageLocation(QDesktopServices::DataLocation);
		QDir dir(myDirName);
		if(!dir.exists()){
			if(!dir.mkpath(myDirName))
				return;
		}
		fileName=myDirName+"/elementCatalogNew.bin";
	}
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QVariant vars;
	QDataStream ds(&file);
	ds>>vars;
	file.close();
	
	//Process Data
	QMap<QString,QVariant> map=vars.toMap();
	rootItem=new CatalogItem(map);
}

void ElementCatalog::save(QString fileName){
	if(fileName==""){
		QString myDirName=QDesktopServices::storageLocation(QDesktopServices::DataLocation);
		QDir dir(myDirName);
		if(!dir.exists()){
			if(!dir.mkpath(myDirName))
				return;
		}
		fileName=myDirName+"/elementCatalogNew.bin";
	}
	QFile file(fileName);
	file.open(QIODevice::WriteOnly);
	QVariant vars;
	vars=rootItem->saveData();
	qDebug()<<vars;
	QDataStream ds(&file);
	ds<<vars;
	file.close();
}

#else

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
	
	QPair<QString, QString> pair;
	foreach(pair, myData){
		if(pair.first==name&&pair.second==value){
			return;
		}
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

void ElementCatalog::load(QString fileName){
	if(fileName==""){
		QString myDirName=QDesktopServices::storageLocation(QDesktopServices::DataLocation);
		QDir dir(myDirName);
		if(!dir.exists()){
			if(!dir.mkpath(myDirName))
				return;
		}
		fileName=myDirName+"/elementCatalog.bin";
	}
	QFile file(fileName);
	file.open(QIODevice::ReadOnly);
	QVariant vars;
	QDataStream ds(&file);
	ds>>vars;
	file.close();
	
	QList<QVariant> list=vars.toList();
	foreach(QVariant pair, list){
		QList<QString> stringList=pair.toStringList();
		addData(stringList[0],stringList[1]);
	}
}

void ElementCatalog::save(QString fileName){
	if(fileName==""){
		QString myDirName=QDesktopServices::storageLocation(QDesktopServices::DataLocation);
		QDir dir(myDirName);
		if(!dir.exists()){
			if(!dir.mkpath(myDirName))
				return;
		}
		fileName=myDirName+"/elementCatalog.bin";
	}
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


#endif

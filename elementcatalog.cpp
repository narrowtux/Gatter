#include "elementcatalog.h"
#include <QMimeData>
#include <QStringList>
#include <QDebug>
#include <QInputDialog>
ElementCatalog::ElementCatalog(QObject *parent) :
		QAbstractListModel(parent)
{
    setSupportedDragActions(Qt::CopyAction);
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
		return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | def;
    } else {
		return Qt::ItemIsDropEnabled | def;
    }
}

bool ElementCatalog::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent){
    Q_UNUSED(column)
    Q_UNUSED(parent)
    if(action==Qt::IgnoreAction){
		return false;
    }
    if(data->hasFormat("text/gatterxml")){
		QString name=QInputDialog::getText(0,tr("Name"), tr("Please enter a name for this template"));
		if(name=="")
			return false;
		addData(name,QString(data->data("text/gatterxml")).toLatin1(),row);
		return true;
    } else {
		return false;
    }
}

Qt::DropActions ElementCatalog::supportedDropActions() const{
    return Qt::CopyAction;
}

QStringList ElementCatalog::mimeTypes() const{
    return QStringList()<<"text/gatterxml";
}

#include "elementcatalog.h"
#include <QMimeData>
ElementCatalog::ElementCatalog(QObject *parent) :
    QAbstractListModel(parent)
{
    setSupportedDragActions(Qt::CopyAction);
}

void ElementCatalog::addData(QString name, QString value){
    beginInsertRows(QModelIndex(),myData.count()-1,myData.count()-1);
    myData<<QPair<QString,QString>(name, value);
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
	return Qt::ItemIsDragEnabled | def;
    } else {
	return def;
    }
}

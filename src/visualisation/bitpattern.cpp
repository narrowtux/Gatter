#include "bitpattern.h"
#include <QBrush>
BitPattern::BitPattern(QObject *parent) :
    QAbstractTableModel(parent)
{
	init(1,1);
}

int BitPattern::rowCount(const QModelIndex &parent) const
{
	if(!parent.isValid()){
		return myBits.count();
	} else {
		return 0;
	}
}

int BitPattern::columnCount(const QModelIndex &parent) const
{
	if(!parent.isValid()){
		if(myBits.count()>0){
			return myBits.at(0).count();
		} else {
			return 0;
		}
	} else {
		return 0;
	}
}

QVariant BitPattern::data(const QModelIndex &index, int role) const
{
	switch(role){
	case Qt::DisplayRole:
		int row, col;
		row = index.row();
		col = index.column();
		if(rowCount(QModelIndex())>row && columnCount(QModelIndex())>col)
			return QString("%0").arg(myBits[row][col]);
		else 
			return QVariant();
		break;
	case Qt::BackgroundRole:
		row = index.row();
		col = index.column();
		if(rowCount(QModelIndex())>row && columnCount(QModelIndex())>col){
			bool value = myBits[row][col];
			if(value)
				return QBrush(Qt::black);
			else
				return QBrush(Qt::white);
		}
		else 
			return QBrush(Qt::white);
		break;
	case Qt::ForegroundRole:
		row = index.row();
		col = index.column();
		if(rowCount(QModelIndex())>row && columnCount(QModelIndex())>col){
			bool value = myBits[row][col];
			if(value)
				return QBrush(Qt::white);
			else
				return QBrush(Qt::black);
		}
		else 
			return QBrush(Qt::black);
		break;
		break;
	default:
		return QVariant();
	}
}

void BitPattern::init(int rows, int columns)
{
	beginResetModel();
	myBits.clear();
	for (int row = 0; row<rows; row++){
		QList<bool> rowlist;
		for(int col = 0; col<columns; col++)
		{
			rowlist<<0;
		}
		myBits<<rowlist;
	}
	endResetModel();
}

bool BitPattern::setData(const QModelIndex &index, const QVariant &value, int role)
{
	switch(role){
	case Qt::DisplayRole:
		myBits[index.row()][index.column()] = value.toBool();
		emit(dataChanged(index, index));
		return true;
		break;
	default:
		return false;
		break;
	}
}

QString BitPattern::toString()
{
	QString ret;
	foreach(QList<bool> row, myBits)
	{
		foreach(bool v, row)
		{
			ret+=QString("%0").arg(v);
		}
		ret+=";";
	}
	return ret;
}

void BitPattern::initFromString(QString string)
{
	beginResetModel();
	myBits.clear();
	QStringList rows = string.split(";");
	foreach(QString row, rows){
		QList<bool> rowList;
		if(row!=""){
			foreach(QChar c, row){
				if(c=='0')
					rowList<<0;
				else
					rowList<<1;
			}
			myBits<<rowList;
		}
	}
	endResetModel();
}

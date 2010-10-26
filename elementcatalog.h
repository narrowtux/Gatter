#ifndef ELEMENTCATALOG_H
#define ELEMENTCATALOG_H

#include <QAbstractListModel>
#include <QPair>
#include <QAbstractItemDelegate>
#include "catalogitem.h"

//Second Implementation: more flexible and with tree support
class ElementCatalog : public QAbstractItemModel
{
    Q_OBJECT

public:
    ElementCatalog(QObject *parent = 0);
    ~ElementCatalog();

    QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QMimeData* mimeData(const QModelIndexList &indexes) const;
	void load(QString fileName="");
	void save(QString fileName="");
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
	
	void addItem(QString name, QString xml);
	
	bool removeRows(int row, int count, const QModelIndex &parent);
private:
    void setupModelData(const QStringList &lines, CatalogItem *parent);

    CatalogItem *rootItem;
	
	Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
};

#endif // ELEMENTCATALOG_H

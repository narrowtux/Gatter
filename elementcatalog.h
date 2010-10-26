#ifndef ELEMENTCATALOG_H
#define ELEMENTCATALOG_H

#include <QAbstractListModel>
#include <QPair>
#include <QAbstractItemDelegate>
#include "catalogitem.h"

//#define USE_OLD_ELEMENTCATALOG

#ifndef USE_OLD_ELEMENTCATALOG
//Second Implementation: more flexible and with tree support
class ElementCatalog : public QAbstractItemModel
{
    Q_OBJECT

public:
    ElementCatalog(QObject *parent = 0);
    ~ElementCatalog();

    QVariant data(const QModelIndex &index, int role) const;
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
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
	
	bool removeRows(int row, int count, const QModelIndex &parent);
private:
    void setupModelData(const QStringList &lines, CatalogItem *parent);

    CatalogItem *rootItem;
	
	Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
};

#else

class ElementCatalog : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElementCatalog(QObject *parent = 0);
	~ElementCatalog();
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
    void addData(QString name, QString value, int row = -1);
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
	bool removeRows(int row, int count, const QModelIndex &parent);
	void load(QString file="");
	void save(QString file="");
signals:

public slots:
private:
    QList< QPair<QString,QString> > myData;
};

//class ElementDelegate : public QAbstractItemDelegate
//{
//	Q_OBJECT
//public:
//	explicit ElementDelegate(QObject*parent=0);
//	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//};
#endif

#endif // ELEMENTCATALOG_H

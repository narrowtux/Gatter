#ifndef ELEMENTCATALOG_H
#define ELEMENTCATALOG_H

#include <QAbstractListModel>
#include <QPair>
#include <QAbstractItemDelegate>

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
signals:

public slots:
private:
    QList< QPair<QString,QString> > myData;
	void load();
	void save();
};

//class ElementDelegate : public QAbstractItemDelegate
//{
//	Q_OBJECT
//public:
//	explicit ElementDelegate(QObject*parent=0);
//	QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
//};

#endif // ELEMENTCATALOG_H

#ifndef ELEMENTCATALOG_H
#define ELEMENTCATALOG_H

#include <QAbstractListModel>
#include <QPair>

class ElementCatalog : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit ElementCatalog(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
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

};

#endif // ELEMENTCATALOG_H

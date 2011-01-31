#ifndef CATALOGITEM_H
#define CATALOGITEM_H
#include <QVariantList>
#include <QKeySequence>
class CatalogItem
{
public:
    CatalogItem(const QList<QVariant> &data, CatalogItem *parent = 0);
	CatalogItem(const QMap<QString,QVariant> &data, CatalogItem *parent=0);
    ~CatalogItem();

    void appendChild(CatalogItem *child);
	void insertChild(int row, CatalogItem *child);
	void removeChild(int row);
    CatalogItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    CatalogItem *parent();
	QString name();
	QString xml();
	bool isLabel();
	bool isPredefined();
	QVariant saveData();
	void setName(QString name);
private:
    QList<CatalogItem*> childItems;
    QList<QVariant> itemData;
	QString itemName;
	QString itemXml;
	bool itemPredefined;
    CatalogItem *parentItem;
	bool myInMenu;
	QKeySequence myShortcut;
};
#endif // CATALOGITEM_H

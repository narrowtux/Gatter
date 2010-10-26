#include "catalogitem.h"
CatalogItem::CatalogItem(const QList<QVariant> &data, CatalogItem *parent)
{
    parentItem = parent;
    itemData = data;
	if(data.count()>=2){
		itemName=data.at(0).toString();
		itemXml=data.at(1).toString();
		if(data.count()>=3)
			itemPredefined=data.at(2).toBool();
	}
}

CatalogItem::CatalogItem(const QMap<QString, QVariant> &data, CatalogItem *parent){
	itemName=data["name"].toString();
	itemXml=data["xml"].toString();
	itemPredefined=data["predefined"].toBool();
	foreach(QVariant c, data["childItems"].toList()){
		appendChild(new CatalogItem(c.toMap(),this));
	}
	parentItem=parent;
}

CatalogItem::~CatalogItem()
{
    qDeleteAll(childItems);
}



void CatalogItem::appendChild(CatalogItem *item)
{
    childItems.append(item);
}

void CatalogItem::insertChild(int row, CatalogItem *child)
{
	childItems.insert(row, child);
}



CatalogItem *CatalogItem::child(int row)
{
    return childItems.value(row);
}



int CatalogItem::childCount() const
{
    return childItems.count();
}



int CatalogItem::columnCount() const
{
    return 1;
}



QVariant CatalogItem::data(int column) const
{
    return itemData.value(column);
}



CatalogItem *CatalogItem::parent()
{
    return parentItem;
}



int CatalogItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<CatalogItem*>(this));

    return 0;
}

bool CatalogItem::isLabel(){
	return itemXml=="";
}

bool CatalogItem::isPredefined(){
	return itemPredefined;
}

QString CatalogItem::name(){
	return itemName;
}

QString CatalogItem::xml(){
	return itemXml;
}

void CatalogItem::removeChild(int row){
	if(childItems.count()>row){
		childItems.removeAt(row);
	}
}

QVariant CatalogItem::saveData()
{
	QVariant ret;
	QMap<QString, QVariant> list;
	list.insert("name",itemName);
	list.insert("xml",itemXml);
	list.insert("predefined", itemPredefined);
	QList<QVariant> children;
	foreach(CatalogItem*c, childItems){
		children<<c->saveData();
	}

	list.insert("childItems", children);
	ret.setValue(list);
	return ret;
}

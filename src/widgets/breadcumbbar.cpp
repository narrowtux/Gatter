#include "breadcumbbar.h"
#include <QHBoxLayout>
BreadCumbBar::BreadCumbBar(QWidget *parent) :
    QWidget(parent)
{
	QHBoxLayout *l = new QHBoxLayout;
	l->setMargin(0);
	l->setSpacing(0);
	setLayout(l);
}

QSize BreadCumbBar::sizeHint() const
{
	return QSize(500,50);
}

void BreadCumbBar::addAction(QAction *action)
{
	QWidget::addAction(action);
	updateActions();
}

void BreadCumbBar::addActions(QList<QAction *> actions)
{
	QWidget::addActions(actions);
	updateActions();
}

void BreadCumbBar::insertAction(QAction *before, QAction *action)
{
	QWidget::insertAction(before, action);
	updateActions();
}

void BreadCumbBar::insertActions(QAction *before, QList<QAction *> actions)
{
	QWidget::insertActions(before, actions);
	updateActions();
}

void BreadCumbBar::removeAction(QAction *action)
{
	QWidget::removeAction(action);
	updateActions();
}

void BreadCumbBar::updateActions()
{
	foreach(BreadCumbItem *item, myItems)
	{
		item->deleteLater();
	}	        
	myItems.clear();
	foreach(QAction *action, actions())
	{
		BreadCumbItem *item = new BreadCumbItem(this);
		layout()->addWidget(item);
		item->setAction(action);
		myItems<<item;
	}
}

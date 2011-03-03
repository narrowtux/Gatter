#include "breadcumbbar.h"
#include <QHBoxLayout>
BreadCumbBar::BreadCumbBar(QWidget *parent) :
    QWidget(parent)
{
	QHBoxLayout *l = new QHBoxLayout;
	l->setMargin(0);
	l->setSpacing(0);
	item = 0;
	setLayout(l);
}

QSize BreadCumbBar::sizeHint() const
{
	return QSize(500,20);
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
	QHBoxLayout *l = static_cast<QHBoxLayout*>(layout());
	BreadCumbItem *left = 0;
	foreach(QAction *action, actions())
	{
		BreadCumbItem *item = new BreadCumbItem(this);
		item->setLeftBreadCumbItem(left);
		if(left)
			left->setRightBreadCumbItem(item);
		item->setAction(action);
		layout()->addWidget(item);
		//l->setAlignment(item, Qt::AlignLeft);
		myItems<<item;
		left = item;
	}
}

QSize BreadCumbBar::minimumSize() const
{
	int w = 0;
	foreach(BreadCumbItem *item, myItems){
		w+=item->minimumSize().width();
	}
	int h = 20;
	return QSize(w,20);
}

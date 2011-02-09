#ifndef BREADCUMBBAR_H
#define BREADCUMBBAR_H

#include <QWidget>
#include "src/widgets/breadcumbitem.h"
class BreadCumbBar : public QWidget
{
    Q_OBJECT
public:
    explicit BreadCumbBar(QWidget *parent = 0);
	QSize sizeHint() const;
	void addAction(QAction *action);
	void addActions(QList<QAction *> actions);
	void insertAction(QAction *before, QAction *action);
	void insertActions(QAction *before, QList<QAction *> actions);
	void removeAction(QAction *action);
signals:
	
public slots:
protected:
	void updateActions();
	QList<BreadCumbItem *> myItems;
};

#endif // BREADCUMBBAR_H

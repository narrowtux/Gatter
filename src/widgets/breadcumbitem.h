#ifndef BREADCUMBITEM_H
#define BREADCUMBITEM_H

#include <QWidget>

class BreadCumbItem : public QWidget
{
    Q_OBJECT
public:
    explicit BreadCumbItem(QWidget *parent = 0);
	QSize sizeHint() const;
	void setAction(QAction *action);
	QAction *action();
signals:

public slots:
private:
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	QAction *myAction;
	bool leftDown;
};

#endif // BREADCUMBITEM_H

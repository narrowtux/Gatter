#ifndef CONNECTIONLINE_H
#define CONNECTIONLINE_H

#include <QGraphicsLineItem>

class ConnectionLine : public QGraphicsLineItem
{
public:
    explicit ConnectionLine(QGraphicsItem *parent = 0);
	QRectF boundingRect() const;
signals:

public slots:
private:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // CONNECTIONLINE_H

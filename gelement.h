#ifndef GELEMENT_H
#define GELEMENT_H

#include <QObject>
#include <QGraphicsItem>
class OElement;

class GElement : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit GElement(QObject *parent = 0, QGraphicsItem* gparent = 0);
	OElement* object();
	void setObject(OElement* object);
signals:

public slots:
	
private:
	OElement* myObject;

};

#endif // GELEMENT_H

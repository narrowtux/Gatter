#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H
#include "element.h"
#include <QVector>

class ShiftRegister : public Element
{
    Q_OBJECT
public:
    explicit ShiftRegister(QGraphicsObject *parent = 0);
	QRectF boundingRect() const;
	void setPrivateXml(QXmlStreamWriter *xml);
	void readPrivateXml(QXmlStreamReader *xml);
signals:

private slots:
	void shift();
	void clock(bool value);
	void recalculate();
protected:
	void connectionsChanged();
	QVector<bool> myRegister;
	bool beforeClockValue;
};

#endif // SHIFTREGISTER_H

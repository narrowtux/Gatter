#ifndef OELEMENT_H
#define OELEMENT_H

#include <QObject>
class GElement;
class OElement : public QObject
{
    Q_OBJECT
public:
    explicit OElement(QObject *parent = 0);
	GElement* graphic();
	void setGraphic(GElement* graphic);
signals:

public slots:

private:
	GElement* myGraphic;
};

#endif // OELEMENT_H

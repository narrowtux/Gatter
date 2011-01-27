#ifndef GENERATOR_H
#define GENERATOR_H
#include "element.h"
#include "src/visualisation/bitpattern.h"
class Generator : public Element
{
    Q_OBJECT
public:
    explicit Generator(QGraphicsObject *parent = 0);
	QRectF boundingRect() const;
signals:

private slots:
	void indexClicked( const QModelIndex & index );
	void rowsChanged(int rows);
	void outputCountChanged(int count);
protected:
	bool createFormBefore();
	BitPattern * myBitPattern;
	int myRows;
	int myOutputCount;
	void setPrivateXml(QXmlStreamWriter *xml);
	void readPrivateXml(QXmlStreamReader *xml);
};

#endif // GENERATOR_H

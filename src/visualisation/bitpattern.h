#ifndef BITPATTERN_H
#define BITPATTERN_H

#include <QAbstractTableModel>

class BitPattern : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BitPattern(QObject *parent = 0);
	QVariant data(const QModelIndex &index, int role) const;
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	void init(int rows, int columns);
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	QString toString();
	void initFromString(QString string);
signals:

public slots:
private:
	QList< QList<bool> > myBits;
};

#endif // BITPATTERN_H

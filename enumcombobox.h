#ifndef ENUMCOMBOBOX_H
#define ENUMCOMBOBOX_H

#include <QComboBox>
#include <QMap>

class EnumComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit EnumComboBox(QWidget *parent = 0);
    void addItems(QMap<int,QString> enumStrings);
signals:
    void currentEnumIndexChanged(int type);
public slots:
    void setCurrentIndex(int type);
    void clear();
private:
    int myCurrentIndex;
    QMap<int,QString> myEnumString;
    QList<int> myIntEnum;
    QStringList myIntString;
private slots:
    void onIndexChanged(int index);
};

#endif // ENUMCOMBOBOX_H

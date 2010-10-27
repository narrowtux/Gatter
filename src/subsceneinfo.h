#ifndef SUBSCENEINFO_H
#define SUBSCENEINFO_H

#include <QObject>
#include <QtCore>
#include <QIcon>

class SubSceneInfo : public QObject
{
    Q_OBJECT
public:
    explicit SubSceneInfo(QObject *parent = 0);
    void setName(QString name);
    void setFileName(QString fileName);
    void setIcon(QIcon icon);
    QString name();
    QString fileName();
    QIcon icon();
    bool isProtected();
    void setProtected(bool p);
signals:

public slots:
protected:
    QString myName;
    QString myFileName;
    QIcon myIcon;
    bool myProtected;
};

#endif // SUBSCENEINFO_H

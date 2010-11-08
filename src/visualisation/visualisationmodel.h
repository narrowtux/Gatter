#ifndef VISUALISATIONMODEL_H
#define VISUALISATIONMODEL_H

#include <QAbstractItemModel>

class VisualisationModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit VisualisationModel(QObject *parent = 0);

signals:

public slots:

};

#endif // VISUALISATIONMODEL_H

#ifndef SUBSCENECHOOSEDIALOG_H
#define SUBSCENECHOOSEDIALOG_H

#include <QDialog>
#include <QtCore>
#include "src/elements/subscene.h"
#include "src/subsceneinfo.h"

namespace Ui {
    class SubSceneChooseDialog;
}

class SubSceneChooseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SubSceneChooseDialog(QWidget *parent = 0);
    ~SubSceneChooseDialog();
    SubScene* getSubScene();
public slots:
    void updateExisting();
    void saveExisting();
private:
    Ui::SubSceneChooseDialog *ui;
    QDir myDirectory;
    QList<SubSceneInfo*> information;
    void showEvent(QShowEvent *);
    QFutureWatcher<bool>* resultWatcher;
private slots:
    void on_toolRemoveSubScene_clicked();
    void on_toolAddSubScene_clicked();
};

#endif // SUBSCENECHOOSEDIALOG_H

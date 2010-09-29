#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "defines.h"
#include <QFormLayout>
class Scene;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QFormLayout* getFormLayout();
	
public slots:

protected:
    void changeEvent(QEvent *e);
	QString fileName;
	static int unnamedIndex;
private:
    Ui::MainWindow *ui;
    static QList<MainWindow*> mainWindows;
    static QList<QAction*> windowActions;
    QAction* myAction;
    Scene* myScene;
    void updateActions();
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
private slots:
    void on_spinDelay_valueChanged(int );
    void on_actionInsertButton_triggered();
    void on_actionDelete_triggered();
    void on_actionInsertLamp_triggered();
    void on_actionInsertSwitch_triggered();
    void on_actionMultiplexer_triggered();
    void on_actionInsertNOT_triggered();
    void on_actionInsertXOR_triggered();
    void on_actionInsertOR_triggered();
    void on_actionInsertAND_triggered();
    void on_actionClose_triggered();
    void on_actionNew_triggered();
    void updateSceneRect();
};

#endif // MAINWINDOW_H

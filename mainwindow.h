#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "defines.h"
#include <QFormLayout>
#include "settingsdialog.h"
#include <QtConcurrentRun>
class SubSceneChooseDialog;
class Scene;
class QXmlStreamReader;
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Scene*scene=0);
    ~MainWindow();
    QFormLayout* getFormLayout();
    void setCurrentFile(const QString &fileName);
	
public slots:
    MainWindow* newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();

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
    void saveFileTo(QString fileName);
    void loadFileFrom(QString fileName);
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    QString curFile;
    SettingsDialog* settingsDialog;
    bool mySubScene, myShouldBeSaved;
    SubSceneChooseDialog* subSceneChooseDialog;
private slots:
    void on_actionInsertFlipflop_triggered();
    void on_actionInsertDelay_triggered();
    void on_actionInsertSubscene_triggered();
    void on_actionLayoutMiddle_triggered();
    void on_actionLayoutCenter_triggered();
    void on_actionLayoutLeft_triggered();
    void on_actionPreferences_triggered();
    void on_actionInsertClock_triggered();
    void on_zoomSlider_valueChanged(int value);
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
    void updateSceneRect();
};

#endif // MAINWINDOW_H

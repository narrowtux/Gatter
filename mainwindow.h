#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "defines.h"
#include <QFormLayout>
#include "settingsdialog.h"
#include <QtConcurrentRun>
#include <QtGui>
#include <QUndoStack>
#include <QUndoView>
#include "elementcatalog.h"

class SubSceneChooseDialog;
class Scene;
class QXmlStreamReader;
class Element;
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
    static QList<MainWindow*> mainWindows;
    Scene* scene();
    void initElementCatalog();
public slots:
    MainWindow* newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void zoomIn();
    void zoomOut();
    void zoomTo(int v);
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);

protected:
    void changeEvent(QEvent *e);
    QString fileName;
    static int unnamedIndex;
    static bool argvFileAlreadyOpened;
private:
    QToolButton *myZoomIn, *myZoomOut;
    qreal scale;
    QSlider *myZoomSlider;
    
    QUndoStack* myUndoStack;
    QUndoView* myUndoView;
    
    Ui::MainWindow *ui;
    static QList<QAction*> windowActions;
    QAction* myAction;
    Scene* myScene;
    void updateActions();
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
    void saveFileTo(QString fileName);
    void loadFileFrom(QString fileName);
    void writeSettings();
    bool maybeSave();
    QString strippedName(const QString &fullFileName);
    QString curFile;
    SettingsDialog* settingsDialog;
    bool mySubScene, myShouldBeSaved;
    SubSceneChooseDialog* subSceneChooseDialog;
    
    static ElementCatalog* elementCatalog;
private slots:
	void addLabel();
	void addFromFile();
    void on_toolRemoveTemplate_clicked();
    void readSettings();
    void elementMoved(QList<Element*> e, QList<QPointF> oldPos);
    void on_actionRotate_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionCopy_triggered();
    void on_actionSelectAll_triggered();
    void on_actionInsertHexOutput_triggered();
    void on_actionInsertFlipflop_triggered();
    void on_actionInsertDelay_triggered();
    void on_actionInsertSubscene_triggered();
    void on_actionLayoutMiddle_triggered();
    void on_actionLayoutCenter_triggered();
    void on_actionLayoutLeft_triggered();
    void on_actionPreferences_triggered();
    void on_actionInsertClock_triggered();
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

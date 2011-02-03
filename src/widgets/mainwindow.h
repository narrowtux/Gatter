#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/defines.h"
#include <QFormLayout>
#include "settingsdialog.h"
#include <QtConcurrentRun>
#include <QtGui>
#include <QUndoStack>
#include <QUndoView>
#include "src/catalog/elementcatalog.h"
#include "src/grapher/utdiagram.h"

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
	QGraphicsRectItem *sceneFitRect;
    
    QUndoStack* myUndoStack;
    QUndoView* myUndoView;
    
    Ui::MainWindow *ui;
    static QList<QAction*> windowActions;
    QAction* myAction;
    Scene* myScene;
    void updateActions();
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *);
    void writeSettings();
    bool maybeSave();
    QString strippedName(const QString &fullFileName);
    QString curFile;
    SettingsDialog* settingsDialog;
    bool mySubScene, myShouldBeSaved;
    SubSceneChooseDialog* subSceneChooseDialog;
    
    static ElementCatalog* elementCatalog;
	QPrintDialog *printDialog;
	
	QList<UTDiagram*> myDiagrams;
	UTDiagram* myDiagram;
	
	QSpinBox *myZoomBox;
private slots:
	void printDialogClosed();
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
    void on_actionInsertSubscene_triggered();
    void on_actionLayoutMiddle_triggered();
    void on_actionLayoutCenter_triggered();
    void on_actionLayoutLeft_triggered();
    void on_actionPreferences_triggered();
    void on_actionDelete_triggered();
    void on_actionClose_triggered();
    void updateSceneRect();
	void on_actionPrint_triggered();
	void updateConnectionAddMenu();
	void connectionAddMenuHover();
	void on_recordUTButton_toggled(bool checked);
	void addUTConnectionRecording();
	void on_autoScrollButton_clicked(bool checked);
	void on_actionInsertByTypeName_triggered();
};

#endif // MAINWINDOW_H

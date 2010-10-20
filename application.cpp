#include "application.h"
#include <QFileOpenEvent>
#include <QDebug>
#include "mainwindow.h"

Application::Application(int &argc, char **argv) :
	QApplication(argc,argv)
{
}

bool Application::event(QEvent *e){
    if(QApplication::event(e)){
	return true;
    } else {
	if(e->type()==QEvent::FileOpen){
	    QFileOpenEvent* event=(QFileOpenEvent*)e;
	    qDebug()<<"File Open"<<event->file();
	    if(MainWindow::mainWindows.count()>0){
		MainWindow::mainWindows.at(0)->loadFile(event->file());
	    }
	    return true;
	} else {
	    return false;
	}
    }
}

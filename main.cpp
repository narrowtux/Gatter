#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "subscenechoosedialog.h"
#include "application.h"
int main(int argc, char *argv[])
{
    Application a(argc, argv);	
	
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("gatter_") + locale,QApplication::applicationDirPath());
    a.installTranslator(&translator);
    qDebug()<<locale;
    a.setOrganizationDomain("de.moritz-schmale");
    a.setApplicationName("Gatter");
    a.setOrganizationName("MoritzSchmale");
#include "mainwindow.h"
    MainWindow w;
    w.show();

    return a.exec();
}

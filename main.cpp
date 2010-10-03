#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);	
	
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("gatter_") + locale);
    a.installTranslator(&translator);
    qDebug()<<locale;
    a.setOrganizationDomain("de.moritz-schmale");
    a.setApplicationName("Gatter");
    a.setOrganizationName("MoritzSchmale");
    MainWindow w;
    w.show();

    return a.exec();
}

#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "subscenechoosedialog.h"
#include "application.h"

#ifdef QT_ARCH_MACOSX
extern void qt_mac_set_native_menubar(bool);
#endif // O_MAC


int main(int argc, char *argv[])
{
    qt_mac_set_native_menubar(true);    
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
	w.initElementCatalog();
    w.show();

    return a.exec();
}

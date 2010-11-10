
#define USE_OLD_ELEMENTCATALOG



#include <QtGui/QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>
#include "src/widgets/subscenechoosedialog.h"
#include "src/application.h"

#include "src/widgets/mainwindow.h"

#ifdef QT_ARCH_MACOSX
extern void qt_mac_set_native_menubar(bool);
#endif // O_MAC

int main(int argc, char *argv[])
{
	srand(-QTime::currentTime().secsTo(QTime(0,0,0)));
#ifdef QT_ARCH_MACOSX
    qt_mac_set_native_menubar(true);    
#endif
    Application a(argc, argv);	
	
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("gatter_") + locale,QApplication::applicationDirPath());
    a.installTranslator(&translator);
    qDebug()<<locale;
    a.setOrganizationDomain("de.moritz-schmale");
    a.setApplicationName("Gatter");
    a.setOrganizationName("MoritzSchmale");
    MainWindow w;
	w.initElementCatalog();
    w.show();

    return a.exec();
}

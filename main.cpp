#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setOrganizationDomain("moritz-schmale.de");
	a.setApplicationName("Gatter");
    MainWindow w;
    w.show();

    return a.exec();
}

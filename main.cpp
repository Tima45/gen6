#include "mainwindow.h"
#include <exception>
#include <time.h>
#include <QApplication>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    a.setApplicationName("Gen");
    a.setApplicationVersion(QString::number(61));
    a.setOrganizationName("Tima");
    MainWindow w;
    w.show();
    return a.exec();
}

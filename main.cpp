#include "mainwindow.h"
#include <exception>
#include <time.h>
#include <QApplication>
#include <QMessageBox>


int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

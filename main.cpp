#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(argc<2?"":argv[1]);
    w.show();
    return a.exec();
}

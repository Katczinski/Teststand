#include "mainwindow.h"
#include <QApplication>
#include <stdio.h>

int main(int argc, char *argv[])
{
    setvbuf(stdin, NULL, _IONBF, 0);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

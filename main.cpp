#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include "sudoku.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("Sudoku"));
    w.show();



    return a.exec();
}

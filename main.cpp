#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(w.geometry().width(),w.geometry().height());
    w.show();
    return a.exec();
}

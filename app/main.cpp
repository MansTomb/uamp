#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QtWidgets/QStyleFactory>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setOrganizationName("uAmp");
    QApplication::setApplicationName("uAmp");

    MainWindow w("uAmp", 0);
    w.setAutoFillBackground(true);
    w.show();

    return a.exec();
}

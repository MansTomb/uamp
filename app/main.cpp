#include <QApplication>
#include <QDebug>

#include "LoginWindow.h"
#include "bass.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setOrganizationName("uAmp");
    QApplication::setApplicationName("uAmp");

    LoginWindow w("uAmp", 0);
    w.setAutoFillBackground(true);
    w.show();

    return a.exec();
}

#include "mainwindow.h"

MainWindow::MainWindow(const QString &name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->setWindowTitle("uText");
    setObjectName(name);
    setWindowTitle(name);
}

MainWindow::~MainWindow() {
    delete ui;
}

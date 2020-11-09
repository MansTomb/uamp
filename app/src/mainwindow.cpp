#include "mainwindow.h"

MainWindow::MainWindow(const QString &name, SqlDatabase &base, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_base(base){
    ui->setupUi(this);

    BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);

    if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
        qDebug() << ("Can't initialize device");
    }
    this->setWindowTitle("uText");
    setObjectName(name);
    setWindowTitle(name);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
}

MainWindow::~MainWindow() {
    BASS_Free();
    delete ui;
}

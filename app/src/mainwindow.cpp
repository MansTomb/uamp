#include "mainwindow.h"

MainWindow::MainWindow(const QString &name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QSettings settings;
    QString theme = settings.value("theme").toString();
    theme.isEmpty() ? theme = "Default" : nullptr;
    QFile File(":/qss/" + theme + ".qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet;

    StyleSheet = QLatin1String(File.readAll());
    qApp->setStyleSheet(StyleSheet);

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


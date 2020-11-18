#include "mainwindow.h"

MainWindow::MainWindow(const QString &name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setSettings();

    BASS_SetConfig(BASS_CONFIG_DEV_DEFAULT, 1);

    if (!BASS_Init(-1, 44100, 0, NULL, NULL)) {
        qDebug() << ("Can't initialize device");
    }
    this->setWindowTitle("uText");
    setObjectName(name);
    setWindowTitle(name);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    m_icon = new QSystemTrayIcon(this);
    m_icon->setIcon(QIcon(":/playerIcons/TrayIcon.svg"));
    m_icon->setToolTip("Uamp" "\n""Uamp Now Executed");
    m_icon->show();
}

MainWindow::~MainWindow() {
    BASS_Free();
    QSettings settings;
    settings.setValue("geometry", geometry());
    delete m_icon;
    delete ui;
}

void MainWindow::setSettings() {
    QSettings settings;

    QStringList keys = settings.allKeys();
    qDebug() << keys;

    QString theme = settings.value("theme").toString();
    theme.isEmpty() ? theme = "Default" : nullptr;
    QFile File(":/qss/" + theme + ".qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet;
    StyleSheet = QLatin1String(File.readAll());
    qApp->setStyleSheet(StyleSheet);
}

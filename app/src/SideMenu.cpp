#include "SideMenu.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

SideMenu::~SideMenu() {
    delete m_eq;
    delete m_radio;
    delete m_timer;
    delete ui;
}

void SideMenu::OpenEqualizerSlot() {
    if (!m_eq) {
        m_eq = new Equalizer(this);
        connect(m_eq, &Equalizer::OnApplyClicked, this, &SideMenu::EqualizerChanged);
    }
    m_eq->open();
}

void SideMenu::OpenRadioSlot() {
    if (!m_radio) {
        m_radio = new Radio(this);
    }
    m_radio->open();
}
void SideMenu::OpenTimerSlot() {
    if (!m_timer) {
        m_timer = new Timer(this);
        connect(m_timer, &Timer::TimerEnded, this, &SideMenu::Shutdown);
    }
    m_timer->open();
}

void SideMenu::ChangeTheme(bool state) {
    if (state) {
        QFile File(":/qss/" + QString("Obit") + ".qss");
        File.open(QFile::ReadOnly);
        QString StyleSheet;

        StyleSheet = QLatin1String(File.readAll());
        qApp->setStyleSheet(StyleSheet);
        qDebug() << "Obit";
    }
    else {
        QFile File(":/qss/" + QString("Default") + ".qss");
        File.open(QFile::ReadOnly);
        QString StyleSheet;

        StyleSheet = QLatin1String(File.readAll());
        qApp->setStyleSheet(StyleSheet);
        qDebug() << "default";
    }
}

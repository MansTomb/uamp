#include "SideMenu.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

SideMenu::~SideMenu() {
    if (m_eq)
        delete m_eq;
    if (m_radio)
        delete m_radio;
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

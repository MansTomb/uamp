#include "SideMenu.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideMenu) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

SideMenu::~SideMenu() {
    delete m_eq;
    delete ui;
}

void SideMenu::OpenEqualizerSlot() {
    if (!m_eq) {
        m_eq = new Equalizer(this);
        connect(m_eq, &Equalizer::OnApplyClicked, this, &SideMenu::EqualizerChanged);
    }
    m_eq->exec();
}

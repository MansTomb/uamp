#include "SideMenu.h"

SideMenu::SideMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideMenu)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

SideMenu::~SideMenu()
{
    delete ui;
}

#include "ToolBar.h"
#include "ui_toolbar.h"

ToolBar::ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ToolBar)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground, true);
}

ToolBar::~ToolBar()
{
    delete ui;
}

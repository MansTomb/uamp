#include "Radio.h"
#include "ui_Radio.h"

Radio::Radio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Radio)
{
    ui->setupUi(this);
}

Radio::~Radio()
{
    delete ui;
}

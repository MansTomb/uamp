#include "Equalizer.h"
#include "ui_Equalizer.h"

Equalizer::Equalizer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Equalizer)
{
    ui->setupUi(this);
}

Equalizer::~Equalizer()
{
    delete ui;
}

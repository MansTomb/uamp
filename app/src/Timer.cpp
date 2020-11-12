#include "Timer.h"

Timer::Timer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Timer)
{
    ui->setupUi(this);

    m_timer = new QTimer(this);
    ui->Time->setValidator(new QIntValidator);
    connect(m_timer, &QTimer::timeout, this, &Timer::TimerEnd);
}

Timer::~Timer()
{
    delete ui;
}

void Timer::StartClicked(bool state) {
    if (state) {
        m_timer->setInterval(ui->Time->text().toInt() * 60000);
        m_timer->start();
        QMessageBox::about(this, "Timer", "Application gonna shutdown at " + ui->Time->text() + " minutes!");
    }
    else {
        m_timer->stop();
        QMessageBox::about(this, "Timer", "Timer stopped!");
    }
}

void Timer::TimerEnd() {
    emit TimerEnded();
}

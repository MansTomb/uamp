#ifndef TIMER_H
#define TIMER_H

#include <QDialog>
#include <QTimer>
#include <QMessageBox>
#include <QIntValidator>

#include "ui_Timer.h"

namespace Ui {
class Timer;
}

class Timer : public QDialog
{
    Q_OBJECT

public:
    explicit Timer(QWidget *parent = nullptr);
    ~Timer();

 public slots:
    void StartClicked(bool state);
    void TimerEnd();

 signals:
    void TimerEnded();

private:
    QTimer *m_timer;
    Ui::Timer *ui;
};

#endif // TIMER_H

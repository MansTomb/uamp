#ifndef RADIO_H
#define RADIO_H

#include <QDialog>

namespace Ui {
class Radio;
}

class Radio : public QDialog
{
    Q_OBJECT

public:
    explicit Radio(QWidget *parent = nullptr);
    ~Radio();

private:
    Ui::Radio *ui;
};

#endif // RADIO_H

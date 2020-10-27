#ifndef SIDEMENU_H
#define SIDEMENU_H

#include <QWidget>

#include "ui_SideMenu.h"

namespace Ui {
class SideMenu;
}

class SideMenu : public QWidget
{
    Q_OBJECT

public:
    explicit SideMenu(QWidget *parent = nullptr);
    ~SideMenu();

private:
    Ui::SideMenu *ui;
};

#endif // SIDEMENU_H

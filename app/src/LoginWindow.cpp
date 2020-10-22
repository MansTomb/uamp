#include "LoginWindow.h"

LoginWindow::LoginWindow(const QString &name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    main = new MainWindow("uAmp", this);
    main->hide();

    this->setWindowTitle("uText");
    setObjectName(name);
    setWindowTitle(name);
}

LoginWindow::~LoginWindow() {
    delete main;
    delete ui;
}

void LoginWindow::onLoginClicked() {
    if (CheckCredentials()) {
        OpenMainWindow();
        emit LoginSuccess();
    }
    else {
        InvalidCredentials();
    }
}

void LoginWindow::onRegisterClicked() {
    if (!CheckUsernameExist())
        InvalidUsername();
    else if (!CheckPasswordConfirm())
        InvalidPassword();
    else
        emit RegisterSuccess();
}

void LoginWindow::OpenMainWindow() {
    main->show();
    hide();
}
void LoginWindow::onGoogleLoginClicked() {
    google.StartAuth();
}

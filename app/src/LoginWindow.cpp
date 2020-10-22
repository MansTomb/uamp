#include "LoginWindow.h"

LoginWindow::LoginWindow(const QString &name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    main = new MainWindow("uAmp", this);
    main->hide();

    this->setWindowTitle("uText");
    setObjectName(name);
    setWindowTitle(name);

    connect(&google, &GoogleAuthWrapper::AuthComplete, this, [=]{OpenMainWindow();});
    connect(this, &LoginWindow::LoginSuccess, this, [=]{OpenMainWindow();});
    connect(this, &LoginWindow::RegisterSuccess, this, &LoginWindow::onRegisterSuccess);
}

LoginWindow::~LoginWindow() {
    delete main;
    delete ui;
}

void LoginWindow::onLoginClicked() {
    if (CheckCredentials()) {
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

void LoginWindow::onGoogleLoginClicked() {
    google.StartAuth();
}

void LoginWindow::OpenMainWindow() {
    main->show();
    hide();
}

void LoginWindow::onRegisterSuccess() {
    ui->tabWidget->setCurrentIndex(0);
}

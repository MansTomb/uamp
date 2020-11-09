#include "LoginWindow.h"

LoginWindow::LoginWindow(const QString &name, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    SqlDatabase::instance().connectDataBase();

    main = new MainWindow("uAmp", this);
    main->hide();

    this->setWindowTitle("uText");
    setObjectName(name);
    setWindowTitle(name);

    //Засетил тему
    QFile File(":/qss/Default.qss");
    File.open(QFile::ReadOnly);
    QString StyleSheet;

    StyleSheet = QLatin1String(File.readAll());
    qApp->setStyleSheet(StyleSheet);

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
    if (CheckInputData())
        emit RegisterSuccess();
}

void LoginWindow::onGoogleLoginClicked() {
    google.StartAuth();
}

void LoginWindow::OpenMainWindow() {
    main->setLogin(m_login);
    main->show();
    hide();
}

void LoginWindow::onRegisterSuccess() {
    ui->tabWidget->setCurrentIndex(0);
}

bool LoginWindow::CheckCredentials() {
    if (SqlDatabase::instance().CheckCredentials(ui->loginUsernameEdit->text(),
                                                 ui->loginPasswordEdit->text())) {
        m_login = ui->loginUsernameEdit->text();
        return true;
    }
    return false;
}

void LoginWindow::InvalidCredentials() {
    QMessageBox::about(this, "Login", "Your login or password isn`t valid!");
}

bool LoginWindow::CheckInputData() {
    QString login = ui->registerUsernameEdit->text();
    QString pass1 = ui->registerPasswordEdit->text();
    QString pass2 = ui->registerPasswordConfEdit->text();

    if (pass1.count() < 6 || pass2.count() < 6 || login.count() < 6) {
        InvalidData();
        return false;
    }
    else if (login == SqlDatabase::instance().getLogin(login)) {
        InvalidUsername();
        return false;
    }
    else if (pass1 != pass2) {
        InvalidPassword();
        return false;
    }

    SqlDatabase::instance().addUserToDataBase(login, pass1);
    return true;
}

void LoginWindow::InvalidPassword() {
    QMessageBox::about(this, "Registration", "Passwords do not match!");
}

void LoginWindow::InvalidUsername() {
    QMessageBox::about(this, "Registration", "This login already exists!");
}

void LoginWindow::InvalidData() {
    QMessageBox::about(this, "Registration", "Your login or password is less than 6 characters long");
}

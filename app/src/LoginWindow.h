#pragma  once

#include <QMainWindow>

#include "ui_loginwindow.h"
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow {
 Q_OBJECT
    public:
        LoginWindow(const QString &name, QWidget *parent = nullptr);
        ~LoginWindow();
    private slots:
        void onLoginClicked();
        void onRegisterClicked();

        void InvalidCredentials() {return;};
        void InvalidPassword() {return;};
        void InvalidUsername() {return;};
    signals:
        void RegisterSuccess();
        void LoginSuccess();
    private:
        bool CheckCredentials() {return true;};
        bool CheckUsernameExist() {return true;};
        bool CheckPasswordConfirm() {return true;};
        void OpenMainWindow();

        Ui::LoginWindow *ui;
        MainWindow *main {Q_NULLPTR};
};

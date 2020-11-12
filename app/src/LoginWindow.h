#pragma  once

#include <QMainWindow>
#include <QWidget>
#include <QSqlQuery>
#include <QMessageBox>

#include "ui_loginwindow.h"
#include "mainwindow.h"
#include "GoogleAuthWrapper.h"
#include "SqlDatabase.h"

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
        void onGoogleLoginClicked();

        void onRegisterSuccess();
        void OpenMainWindow();
 signals:
        void RegisterSuccess();
        void LoginSuccess();
    private:
        bool CheckInputData();
        bool CheckCredentials();
        void InvalidCredentials();
        void InvalidPassword();
        void InvalidUsername();
        void InvalidData();

        Ui::LoginWindow *ui;
        GoogleAuthWrapper google;
        MainWindow *main {Q_NULLPTR};
        QString m_login;
};

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QTableWidgetItem>
#include <QtWidgets/QDialog>
#include <QtWidgets/QStyleFactory>
#include <QLineEdit>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFontDatabase>

#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
 Q_OBJECT

 public:
    MainWindow(const QString &name, QWidget *parent = nullptr);
    ~MainWindow();
 private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
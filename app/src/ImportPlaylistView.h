#pragma once

#include <QDialog>
#include <QErrorMessage>
#include <QFileDialog>

#include "ui_ImportPlaylistView.h"

namespace Ui {
class ImportPlaylistView;
}

class ImportPlaylistView : public QDialog
{
    Q_OBJECT
public:
    explicit ImportPlaylistView(QWidget *parent = nullptr);
    ~ImportPlaylistView();

public slots:
    void Accepted();
    void OpenFileChooser();
private:
    Ui::ImportPlaylistView *ui;
};


#ifndef EDITFILETAGSDIALOG_H
#define EDITFILETAGSDIALOG_H

#include <QDialog>

namespace Ui {
class EditFileTagsDialog;
}

class EditFileTagsDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditFileTagsDialog(QWidget *parent = nullptr);
    ~EditFileTagsDialog();

private:
    Ui::EditFileTagsDialog *ui;
};

#endif // EDITFILETAGSDIALOG_H

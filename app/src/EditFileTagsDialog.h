#ifndef EDITFILETAGSDIALOG_H
#define EDITFILETAGSDIALOG_H

#include <QDialog>

#include "FileTag.h"

namespace Ui {
class EditFileTagsDialog;
}

class EditFileTagsDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditFileTagsDialog(FileTags& file, QWidget *parent = nullptr);
    ~EditFileTagsDialog();

private:
    Ui::EditFileTagsDialog *ui;
};

#endif // EDITFILETAGSDIALOG_H

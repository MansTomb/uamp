#ifndef EDITFILETAGSDIALOG_H
#define EDITFILETAGSDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>

#include "FileTag.h"

namespace Ui {
class EditFileTagsDialog;
}

class EditFileTagsDialog : public QDialog {
    Q_OBJECT

    public:
    explicit EditFileTagsDialog(FileTags& file, QWidget *parent = nullptr);
    ~EditFileTagsDialog();


private slots:
    void on_pushButton_clicked();

private:
    void accept() override;

    FileTags& m_file;
    QString m_pathToImg;
    bool m_changeImg = false;
    Ui::EditFileTagsDialog *ui;
};

#endif // EDITFILETAGSDIALOG_H

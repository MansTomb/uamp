#include "ImportPlaylistView.h"
#include "PlaylistListView.h"

ImportPlaylistView::ImportPlaylistView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportPlaylistView) {
    ui->setupUi(this);
}

ImportPlaylistView::~ImportPlaylistView() {
    delete ui;
}

void ImportPlaylistView::OpenFileChooser() {
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Import Playlist"), "/", tr("Playlist (*.m3u)"));

    ui->label->setText(fileName);
}

void ImportPlaylistView::Accepted() {
    if (ui->label->text().isEmpty()) {
        auto msg = new QErrorMessage(this);
        msg->showMessage("You need to choose m3u file!");
        msg->exec();
        return;
    }

    if (ui->lineEdit->text().isEmpty()) {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Playlist must have a name!");
        msg->exec();
        return;
    }

    if (dynamic_cast<PlaylistListView *>(parent())->ImportNewPlaylist(ui->label->text(), ui->lineEdit->text()))
        close();
    else {
        auto msg = new QErrorMessage(this);
        msg->showMessage("Playlist with this name already exists!");
        msg->exec();
    }
}

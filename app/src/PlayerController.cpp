#include "PlayerController.h"

PlayerController::PlayerController() {

}

void PlayerController::SetSong(const FileTags *song) {
    sample = BASS_StreamCreateFile(FALSE, song->tags.path.toStdString().c_str(), 0, 0, 0);
}

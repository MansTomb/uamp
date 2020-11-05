#include "PlayerController.h"

PlayerController::PlayerController() {

}

void PlayerController::SetSong(const FileTags *song) {
    BASS_StreamFree(sample);
    sample = BASS_StreamCreateFile(FALSE, song->tags.path.toStdString().c_str(), 0, 0, BASS_ASYNCFILE);
}

#pragma once

#include <QWidget>
#include <QDebug>

#include "ui_toolbar.h"

namespace Ui {
class ToolBar;
}

class ToolBar : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBar(QWidget *parent = nullptr);
    ~ToolBar();

 public slots:
    void volumeChanged(int value) {emit VolumeValue(value);};
    void shuffleClicked(bool state) {emit Shuffle(state);};
    void searchClicked() {emit Search();};
    void reposClicked() {emit Reposition();};
    void repeatClicked(bool state) {emit Repeat(state);};
    void repeatSongClicked(bool state) {emit RepeatSong(state);};
    void prevClicked() {emit Previous();};
    void playClicked(bool state) {emit Play(state);};
    void stopButtonClicked() {emit Stop(); ui->PauseBtn->setChecked(false);};
    void nextClicked() {emit Next();};
    void muteClicked(bool state) {emit Mute(state);};
    void forwardClicked() {emit Forward();};
    void delClicked() {emit DeleteSong();};
    void backwardClicked() {emit Backward();};
    void addClicked() {emit AddSong();};
    void menuClicked() {emit Menu();};

    void menuPlay() {emit Play(true); ui->PauseBtn->setChecked(true);};
    void menuStop() {emit Stop(); ui->PauseBtn->setChecked(false);};
    void menuPause() {emit Play(false); ui->PauseBtn->setChecked(false);};
    void menuForward() {emit Forward();};
    void menuRewind() {emit Backward();};
    void menuShuffle(bool state) {emit Shuffle(state); ui->ShuffleBtn->setChecked(state);};
    void menuLoopSong(bool state) {emit RepeatSong(state); ui->RepeatSongBtn->setChecked(state);};
    void menuLoopPlaylist(bool state) {emit Repeat(state); ui->RepeatBtn->setChecked(state);};
    void menuVolumeUp() {emit VolumeValue(ui->VolumeSlider->value() + 10); ui->VolumeSlider->setValue(ui->VolumeSlider->value() + 10);};
    void menuVolumeDown() {emit VolumeValue(ui->VolumeSlider->value() - 10); ui->VolumeSlider->setValue(ui->VolumeSlider->value() - 10);};

signals:
    void VolumeValue(int);
    void Shuffle(bool);
    void RepeatSong(bool);
    void Repeat(bool);
    void Play(bool);
    void Stop();
    void Previous();
    void Backward();
    void Forward();
    void Next();
    void AddSong();
    void DeleteSong();
    void Reposition();
    void Search();
    void Menu();
    void Mute(bool state);
 private:
    Ui::ToolBar *ui;
};

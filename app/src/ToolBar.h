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
    void prevClicked() {emit Previous();};
    void playClicked(bool state) {emit Play(state);};
    void nextClicked() {emit Next();};
    void muteClicked(bool state) {emit Mute(state);};
    void forwardClicked() {emit Forward();};
    void delClicked() {emit DeleteSong();};
    void backwardClicked() {emit Backward();};
    void addClicked() {emit AddSong();};
    void menuClicked() {emit Menu();};

signals:
    void VolumeValue(int);
    void Shuffle(bool);
    void Repeat(bool);
    void Play(bool);
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

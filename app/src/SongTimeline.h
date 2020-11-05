#pragma once

#include <QSlider>
#include <QDebug>

class SongTimeline : public QSlider{
    Q_OBJECT
 protected:
    void mousePressEvent(QMouseEvent *ev) override {
        QSlider::mousePressEvent(ev);
        emit SliderClicked();
    }
signals:
    void SliderClicked();
 public:
    explicit SongTimeline(QWidget *parent = Q_NULLPTR) : QSlider(parent) {};
};

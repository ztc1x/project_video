#include "canvas.h"

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <iostream>

canvas::canvas() : QLabel()
{
}

void canvas::mousePressEvent(QMouseEvent *ev)
{
    if(state != 0x00)
    {
        QPainter* painter = new QPainter();
        painter -> begin(&frame);
        painter -> drawEllipse(QPoint(ev -> x(), ev -> y()), 3, 3);
        painter -> end();
        QPixmap tmp;
        tmp.convertFromImage(frame);
        this -> setPixmap(tmp);
        this -> update();
        emit sig_point_marked(ev -> x(), ev -> y());
    }
    return;
}

void canvas::slot_set_image(QImage image)
{
    frame = image;
    QPixmap tmp;
    tmp.convertFromImage(frame);
    this -> setPixmap(tmp);
    this -> update();
    return;
}

void canvas::slot_change_state(int new_state)
{
    state = new_state;
    return;
}

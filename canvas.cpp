#include "canvas.h"

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <iostream>

canvas::canvas() : QLabel()
{
    state = 0x00;
    coord_orig = false;
    coord_x = false;
    coord_y = false;
    coord_z = false;
}

void canvas::mousePressEvent(QMouseEvent *ev)
{
    if(state == 0x00)
        return;

    QPainter* painter = new QPainter();
    painter -> begin(&frame);
    painter -> drawEllipse(QPoint(ev -> x(), ev -> y()), 3, 3);
    if(state == 0x01)
    {
        coord_orig = true;
        coord_orig_x = ev -> x();
        coord_orig_y = ev -> y();
        if(coord_x == true)
            painter -> drawLine(coord_orig_x, coord_orig_y, coord_x_x, coord_x_y);
        if(coord_y == true)
            painter -> drawLine(coord_orig_x, coord_orig_y, coord_y_x, coord_y_y);
        if(coord_z == true)
            painter -> drawLine(coord_orig_x, coord_orig_y, coord_z_x, coord_z_y);
    }
    if(state == 0x02)
    {
        coord_x = true;
        coord_x_x = ev -> x();
        coord_x_y = ev -> y();
        if(coord_orig == true)
            painter -> drawLine(coord_orig_x, coord_orig_y, coord_x_x, coord_x_y);
    }
    if(state == 0x03)
    {
        coord_y = true;
        coord_y_x = ev -> x();
        coord_y_y = ev -> y();
        if(coord_orig == true)
            painter -> drawLine(coord_orig_x, coord_orig_y, coord_y_x, coord_y_y);
    }
    if(state == 0x04)
    {
        coord_z = true;
        coord_z_x = ev -> x();
        coord_z_y = ev -> y();
        if(coord_orig == true)
            painter -> drawLine(coord_orig_x, coord_orig_y, coord_z_x, coord_z_y);
    }
    painter -> end();
    QPixmap tmp;
    tmp.convertFromImage(frame);
    this -> setPixmap(tmp);
    this -> update();
    emit sig_point_marked(ev -> x(), ev -> y());
    return;
}

void canvas::slot_set_image(QImage image)
{
    frame = image;
    QPixmap tmp;
    tmp.convertFromImage(frame);
    this -> setPixmap(tmp);
    this -> update();

    coord_orig = false;
    coord_x = false;
    coord_y = false;
    coord_z = false;

    return;
}

void canvas::slot_change_state(int new_state)
{
    state = new_state;
    return;
}

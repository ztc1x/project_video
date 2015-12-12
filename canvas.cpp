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
    next_x = ev -> x();
    next_y = ev -> y();
    //this -> update();
    QPainter* painter = new QPainter();
    painter -> begin(&frame);
    painter -> drawEllipse(QPoint(next_x, next_y), 3, 3);
    painter -> end();
    QPixmap tmp;
    tmp.convertFromImage(frame);
    this -> setPixmap(tmp);
    this -> update();
    std::cout << "Mouse clicked at (" << ev -> x() << ", " << ev -> y() << ")" << std::endl;
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

#include "canvas.h"

#include <QLabel>
#include <QMouseEvent>

canvas::canvas() : QLabel()
{

}

void canvas::mousePressEvent(QMouseEvent *ev)
{
    emit sig_mark_point(ev -> x(), ev -> y());
    return;
}

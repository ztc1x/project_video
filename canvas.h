#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>
#include <QObject>

class canvas : public QLabel
{
    Q_OBJECT
public:
    canvas();
protected:
    void mousePressEvent(QMouseEvent *ev);
signals:
    void sig_mark_point(int x, int y);
};

#endif // CANVAS_H

#ifndef CANVAS_H
#define CANVAS_H

#include <QLabel>
#include <QMouseEvent>
#include <QObject>
#include <QImage>

class canvas : public QLabel
{
    Q_OBJECT
public:
    canvas();
private:
    QImage frame;
    int state;
protected:
    void mousePressEvent(QMouseEvent *ev);
signals:
    void sig_point_marked(int x, int y);
public slots:
    void slot_set_image(QImage image);
    void slot_change_state(int new_state);
};

#endif // CANVAS_H

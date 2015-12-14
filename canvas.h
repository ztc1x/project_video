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
    int coord_orig_x, coord_orig_y;
    int coord_x_x, coord_x_y;
    int coord_y_x, coord_y_y;
    int coord_z_x, coord_z_y;
    bool coord_orig, coord_x, coord_y, coord_z;
protected:
    void mousePressEvent(QMouseEvent *ev);
signals:
    void sig_point_marked(int x, int y);
public slots:
    void slot_set_image(QImage image);
    void slot_change_state(int new_state);
};

#endif // CANVAS_H

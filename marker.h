#ifndef MARKER_H
#define MARKER_H

#include <QObject>

class marker : public QObject
{
    Q_OBJECT
public:
    explicit marker(QObject *parent = 0);
    int get_state();
private:
    int state;
    int vid_width, vid_height;
    int port_width, port_height;

    int coord_orig_x, coord_orig_y;
    int coord_x_x, coord_x_y;
    int coord_y_x, coord_y_y;
    int coord_z_x, coord_z_y;
signals:
    void sig_point_marked(int x, int y);
public slots:
    void slot_point_marked(int x, int y);
    void slot_change_state(int new_state);
    void slot_set_scale(int vid_w, int vid_h, int port_w, int port_h);
};

#endif // MARKER_H

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
    int coord_orig_x, coord_orig_y;
    int coord_x_x, coord_x_y;
    int coord_y_x, coord_y_y;
    int coord_z_x, coord_z_y;
signals:

public slots:
    void slot_point_marked(int x, int y);
    void slot_change_state(int new_state);
};

#endif // MARKER_H

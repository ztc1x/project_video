#include "marker.h"
#include <iostream>
using namespace std;

marker::marker(QObject *parent) : QObject(parent)
{
    state = 0x00;
}

int marker::get_state()
{
    return state;
}

void marker::slot_change_state(int new_state)
{
    state = new_state;
}

void marker::slot_point_marked(int x, int y)
{
    if(state == 0x00)
        return;

    int real_x = (int)((double)x / (double)port_width * (double)vid_width);
    int real_y = (int)((double)y / (double)port_height * (double)vid_height);
    emit sig_point_marked(real_x, real_y);

    if(state == 0x01)
    {
        coord_orig_x = real_x;
        coord_orig_y = real_y;
        cout << "Origin marked: (" << real_x << ", " << real_y << ")" << endl;
    }
    else if(state == 0x02)
    {
        coord_x_x = real_x;
        coord_x_y = real_y;
        cout << "X axis marked: (" << real_x << ", " << real_y << ")" << endl;
    }
    else if(state == 0x03)
    {
        coord_y_x = real_x;
        coord_y_y = real_y;
        cout << "Y axis marked: (" << real_x << ", " << real_y << ")" << endl;
    }
    else if(state == 0x04)
    {
        coord_z_x = real_x;
        coord_z_y = real_y;
        cout << "Z axis marked: (" << real_x << ", " << real_y << ")" << endl;
    }

    return;
}

void marker::slot_set_scale(int vid_w, int vid_h, int port_w, int port_h)
{
    vid_width = vid_w;
    vid_height = vid_h;
    port_width = port_w;
    port_height = port_h;

    return;
}

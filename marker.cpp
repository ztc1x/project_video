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
    if(state == 0x01)
    {
        coord_orig_x = x;
        coord_orig_y = y;
        cout << "Origin marked: (" << x << ", " << y << ")" << endl;
    }
    else if(state == 0x02)
    {
        coord_x_x = x;
        coord_x_y = y;
        cout << "X axis marked: (" << x << ", " << y << ")" << endl;
    }
    else if(state == 0x03)
    {
        coord_y_x = x;
        coord_y_y = y;
        cout << "Y axis marked: (" << x << ", " << y << ")" << endl;
    }
    else if(state == 0x04)
    {
        coord_z_x = x;
        coord_z_y = y;
        cout << "Z axis marked: (" << x << ", " << y << ")" << endl;
    }
    return;
}

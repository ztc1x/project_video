#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QWidget>
#include <QToolBar>
#include <QFileDialog>
#include <QPushButton>
#include <QToolButton>
#include <QThread>
#include <QLabel>
#include <QSlider>
#include <QGroupBox>
#include <QVector>
#include <QStackedLayout>
#include <QLineEdit>
#include <QPainter>
#include "video_handler.h"
#include "canvas.h"
#include "marker.h"

namespace Ui {
class main_window;
}

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = 0);
    ~main_window();

private:
    QVBoxLayout* main_layout;
    QHBoxLayout* tools_layout;
    QVBoxLayout* panel_layout;
    QVBoxLayout* dummy_layout;

    QGroupBox* preview_frame;
    QHBoxLayout* preview_layout;
    QVBoxLayout* preview_inner_layout;
    canvas* preview_label;

    QGroupBox* tool_frame;
    QVBoxLayout* tool_layout;
    QStackedLayout* toolset_layout;
    QWidget* coordinate_widget;
    QVBoxLayout* coordinate_layout;
    QGridLayout* coordinate_inner_layout;
    QPushButton *origin_btn, *x_btn, *y_btn, *z_btn;
    QLineEdit *origin_disp, *x_disp, *y_disp, *z_disp;
    QWidget* cuboid_widget;
    QVBoxLayout* cuboid_layout;
    QGridLayout* cuboid_inner_layout;
    QPushButton* points_btn[8];
    QLineEdit* points_disp[8];

    QGroupBox* info_frame;
    QVBoxLayout* info_layout;
    QLabel* info_size;
    QLabel* info_fps;
    QLabel* info_duration;

    QGroupBox* thumbnail_frame;
    QHBoxLayout* timeline_layout;
    QHBoxLayout* thumbnail_layout;
    QVBoxLayout* timeline_inner_layout;
    QLabel* thumbnail_label[10];
    QSlider* seek_slider;

    QWidget* main_widget;

    QToolBar* file_toolbar;
    QToolButton* open_file_btn;

    QToolBar* ops_toolbar;
    QToolButton* coordinate_btn;
    QToolButton* cuboid_btn;

    QFileDialog* open_video_dialog;

    QThread* handler_thread;
    video_handler* handler;

    marker* recorder;

    int state;
public slots:
    void slot_open_video_file();
    void slot_update_progress(int value);
    void slot_reset_slider();
    void slot_load_timeline(QPixmap* thumbnails, int cnt);
    void slot_show_info(int width, int height, int fps, double duration);
    void slot_switch_coordinate_interface();
    void slot_switch_cuboid_interface();

    void slot_set_state_coord_orig();
    void slot_set_state_coord_x();
    void slot_set_state_coord_y();
    void slot_set_state_coord_z();
    void slot_point_marked(int x, int y);
    void slot_release_buttons();
    void slot_reset_state();
signals:
    void sig_progress_changed(double progress);
    void sig_state_changed(int new_state);
};

#endif // MAIN_WINDOW_H

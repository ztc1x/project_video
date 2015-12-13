#include "main_window.h"
#include <iostream>
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
#include <QPixmap>
#include <QSpacerItem>
#include <QGroupBox>
#include <QTextEdit>
#include <QLineEdit>
#include <QStackedLayout>
#include <QPainter>
#include <QPoint>
#include <QString>
#include "video_handler.h"
#include "canvas.h"
#include "marker.h"
using namespace std;

main_window::main_window(QWidget *parent) :
    QMainWindow(parent)
{
    state = 0x00;

    recorder = new marker();

    this -> setFixedSize(960, 660);
    this -> setWindowTitle("Video Marker");

    handler_thread = new QThread();
    handler = new video_handler();
    handler -> moveToThread(handler_thread);

    main_layout = new QVBoxLayout();
    tools_layout = new QHBoxLayout();

    preview_label = new canvas();
    preview_layout = new QHBoxLayout();
    preview_inner_layout = new QVBoxLayout();
    preview_inner_layout -> addStretch();
    preview_inner_layout -> addWidget(preview_label);
    preview_inner_layout -> addStretch();
    preview_layout -> addStretch();
    preview_layout -> addLayout(preview_inner_layout);
    preview_layout -> addStretch();
    preview_frame = new QGroupBox("Preview");
    preview_frame -> setLayout(preview_layout);
    preview_frame -> setFixedSize(640, 480);

    tool_frame = new QGroupBox("Actions");
    toolset_layout = new QStackedLayout();

    coordinate_widget = new QWidget();
    coordinate_layout = new QVBoxLayout();
    coordinate_inner_layout = new QGridLayout();
    origin_btn = new QPushButton("Origin");
    origin_disp = new QLineEdit();
    coordinate_inner_layout -> addWidget(origin_btn, 0, 0, 1, 1);
    coordinate_inner_layout -> addWidget(origin_disp, 0, 1, 1, 1);
    x_btn = new QPushButton("X Axis");
    x_disp = new QLineEdit();
    coordinate_inner_layout -> addWidget(x_btn, 1, 0, 1, 1);
    coordinate_inner_layout -> addWidget(x_disp, 1, 1, 1, 1);
    y_btn = new QPushButton("Y Axis");
    y_disp = new QLineEdit();
    coordinate_inner_layout -> addWidget(y_btn, 2, 0, 1, 1);
    coordinate_inner_layout -> addWidget(y_disp, 2, 1, 1, 1);
    z_btn = new QPushButton("Z Axis");
    z_disp = new QLineEdit();
    coordinate_inner_layout -> addWidget(z_btn, 3, 0, 1, 1);
    coordinate_inner_layout -> addWidget(z_disp, 3, 1, 1, 1);
    coordinate_layout -> addStretch();
    coordinate_layout -> addLayout(coordinate_inner_layout);
    coordinate_layout -> addStretch();
    coordinate_widget -> setLayout(coordinate_layout);
    toolset_layout -> addWidget(coordinate_widget);

    cuboid_widget = new QWidget();
    cuboid_layout = new QVBoxLayout();
    cuboid_inner_layout = new QGridLayout();
    for(int i = 0; i < 8; i ++)
    {
        points_btn[i] = new QPushButton("Point");
        points_disp[i] = new QLineEdit();
        cuboid_inner_layout -> addWidget(points_btn[i], i, 0, 1, 1);
        cuboid_inner_layout -> addWidget(points_disp[i], i, 1, 1, 1);
    }
    cuboid_layout -> addStretch();
    cuboid_layout -> addLayout(cuboid_inner_layout);
    cuboid_layout -> addStretch();
    cuboid_widget -> setLayout(cuboid_layout);
    toolset_layout -> addWidget(cuboid_widget);

    tool_frame -> setLayout(toolset_layout);

    thumbnail_frame = new QGroupBox("Timeline");
    thumbnail_layout = new QHBoxLayout();
    for(int i = 0; i < 10; i ++)
    {
        thumbnail_label[i] = new QLabel();
        thumbnail_label[i] -> setFixedWidth(88);
        thumbnail_layout -> addWidget(thumbnail_label[i]);
    }
    thumbnail_layout -> addStretch();
    timeline_inner_layout = new QVBoxLayout();
    timeline_inner_layout -> addStretch();
    timeline_inner_layout -> addLayout(thumbnail_layout);
    seek_slider = new QSlider(Qt::Horizontal);
    seek_slider -> setRange(0, 999);
    seek_slider -> setTracking(false);
    seek_slider -> setEnabled(false);
    timeline_inner_layout -> addWidget(seek_slider);
    timeline_inner_layout -> addStretch();
    timeline_layout = new QHBoxLayout();
    timeline_layout -> addStretch();
    timeline_layout -> addLayout(timeline_inner_layout);
    timeline_layout -> addStretch();
    thumbnail_frame -> setLayout(timeline_layout);
    tools_layout -> addWidget(preview_frame);
    tools_layout -> addWidget(tool_frame);
    main_layout -> addLayout(tools_layout);
    main_layout -> addWidget(thumbnail_frame);

    main_widget = new QWidget();
    main_widget -> setLayout(main_layout);
    this -> setCentralWidget(main_widget);

    file_toolbar = new QToolBar("Files");
    open_file_btn = new QToolButton(file_toolbar);
    open_file_btn -> setText("Open...");
    connect(open_file_btn, SIGNAL(clicked(bool)), this, SLOT(slot_open_video_file()));
    file_toolbar -> addWidget(open_file_btn);
    file_toolbar -> setFloatable(false);
    file_toolbar -> setMovable(false);
    this -> addToolBar(file_toolbar);

    ops_toolbar = new QToolBar("Operations");
    coordinate_btn = new QToolButton(ops_toolbar);
    coordinate_btn -> setText("Coordinates...");
    cuboid_btn = new QToolButton(ops_toolbar);
    cuboid_btn -> setText("Cuboid...");
    ops_toolbar -> addWidget(coordinate_btn);
    ops_toolbar -> addWidget(cuboid_btn);
    ops_toolbar -> setFloatable(false);
    ops_toolbar -> setMovable(false);
    this -> addToolBar(ops_toolbar);

    open_video_dialog = new QFileDialog();

    connect(open_video_dialog, SIGNAL(fileSelected(QString)), handler, SLOT(slot_open_video(QString)));
    connect(open_video_dialog, SIGNAL(fileSelected(QString)), this, SLOT(slot_reset_slider()));
    connect(handler, SIGNAL(sig_preview_image(QImage)), preview_label, SLOT(slot_set_image(QImage)));
    connect(handler, SIGNAL(sig_timeline(QPixmap*, int)), this, SLOT(slot_load_timeline(QPixmap*, int)));
    connect(seek_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_update_progress(int)));
    connect(this, SIGNAL(sig_progress_changed(double)), handler, SLOT(slot_update_preview(double)));

    connect(coordinate_btn, SIGNAL(clicked(bool)), this, SLOT(slot_switch_coordinate_interface()));
    connect(cuboid_btn, SIGNAL(clicked(bool)), this, SLOT(slot_switch_cuboid_interface()));

    connect(origin_btn, SIGNAL(pressed()), this, SLOT(slot_set_state_coord_orig()));
    connect(x_btn, SIGNAL(pressed()), this, SLOT(slot_set_state_coord_x()));
    connect(y_btn, SIGNAL(pressed()), this, SLOT(slot_set_state_coord_y()));
    connect(z_btn, SIGNAL(pressed()), this, SLOT(slot_set_state_coord_z()));
    connect(this, SIGNAL(sig_state_changed(int)), preview_label, SLOT(slot_change_state(int)));
    connect(this, SIGNAL(sig_state_changed(int)), recorder, SLOT(slot_change_state(int)));
    connect(preview_label, SIGNAL(sig_point_marked(int,int)), this, SLOT(slot_point_marked(int,int)));
    connect(preview_label, SIGNAL(sig_point_marked(int,int)), recorder, SLOT(slot_point_marked(int,int)));
    connect(preview_label, SIGNAL(sig_point_marked(int,int)), this, SLOT(slot_release_button()));

    handler_thread -> start();
}

main_window::~main_window()
{
}

void main_window::slot_open_video_file()
{
    open_video_dialog -> exec();
    return;
}

void main_window::slot_update_progress(int value)
{
    emit sig_progress_changed((double)value / 1000.0);
}

void main_window::slot_reset_slider()
{
    seek_slider -> setEnabled(true);
    seek_slider -> setValue(0);
    return;
}

void main_window::slot_load_timeline(QPixmap* thumbnails, int cnt)
{
    for(int i = 0; i < cnt; i ++)
    {
        thumbnail_label[i] -> setPixmap(thumbnails[i]);
    }
    return;
}

void main_window::slot_switch_coordinate_interface()
{
    toolset_layout -> setCurrentIndex(0);
    return;
}

void main_window::slot_switch_cuboid_interface()
{
    toolset_layout -> setCurrentIndex(1);
    return;
}

void main_window::slot_set_state_coord_orig()
{
    state = 0x01;
    origin_btn -> setDisabled(true);
    emit sig_state_changed(state);
    return;
}

void main_window::slot_set_state_coord_x()
{
    state = 0x02;
    x_btn -> setDisabled(true);
    emit sig_state_changed(state);
    return;
}

void main_window::slot_set_state_coord_y()
{
    state = 0x03;
    y_btn -> setDisabled(true);
    emit sig_state_changed(state);
    return;
}

void main_window::slot_set_state_coord_z()
{
    state = 0x04;
    z_btn -> setDisabled(true);
    emit sig_state_changed(state);
    return;
}

void main_window::slot_point_marked(int x, int y)
{
    QString text = "(";
    text += QString::number(x);
    text += ", ";
    text += QString::number(y);
    text += ")";

    if(state == 0x01)
        origin_disp -> setText(text);
    else if(state == 0x02)
        x_disp -> setText(text);
    else if(state == 0x03)
        y_disp -> setText(text);
    else if(state == 0x04)
        z_disp -> setText(text);

    return;
}

void main_window::slot_release_button()
{
    if(state == 0x01)
        origin_btn -> setDisabled(false);
    else if(state == 0x02)
        x_btn -> setDisabled(false);
    else if(state == 0x03)
        y_btn -> setDisabled(false);
    else if(state == 0x04)
        z_btn -> setDisabled(false);

    state = 0x00;
    emit sig_state_changed(state);

    return;
}

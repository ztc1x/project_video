#include "main_window.h"
#include "video_handler.h"
#include "canvas.h"
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
using namespace std;

main_window::main_window(QWidget *parent) :
    QMainWindow(parent)
{
    state = 0x00;

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

    marker = new QPainter();
    QBrush brush;
    brush.setColor(QColor(255, 255, 255));
    //marker -> setBrush(brush);

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
    connect(preview_label, SIGNAL(sig_mark_point(int,int)), this, SLOT(slot_mark_point(int,int)));
    connect(origin_btn, SIGNAL(clicked(bool)), this, SLOT(slot_set_state_coord_orig()));

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

void main_window::slot_mark_point(int x, int y)
{
    if(state != 0x00)
    {
        marker -> begin(preview_label);
        QPoint point(x, y);
        marker -> drawEllipse(point, 3, 3);
        marker -> end();
    }
    return;
}

void main_window::slot_set_state_coord_orig()
{
    state = 0x01;
    return;
}

#include "main_window.h"
#include "video_handler.h"
#include <iostream>
#include <QHBoxLayout>
#include <QVBoxLayout>
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
#include <QVector>
using namespace std;

main_window::main_window(QWidget *parent) :
    QMainWindow(parent)
{
    this -> setFixedSize(960, 660);
    this -> setWindowTitle("Video Marker");

    handler_thread = new QThread();
    handler = new video_handler();
    handler -> moveToThread(handler_thread);

    main_layout = new QVBoxLayout();
    tools_layout = new QHBoxLayout();

    preview_label = new QLabel();
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
    connect(handler, SIGNAL(sig_preview(QPixmap)), preview_label, SLOT(setPixmap(QPixmap)));
    connect(handler, SIGNAL(sig_timeline(QPixmap*, int)), this, SLOT(slot_load_timeline(QPixmap*, int)));
    connect(seek_slider, SIGNAL(valueChanged(int)), this, SLOT(slot_update_progress(int)));
    connect(this, SIGNAL(sig_progress_changed(double)), handler, SLOT(slot_update_preview(double)));

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

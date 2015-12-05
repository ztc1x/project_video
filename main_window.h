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
#include "video_handler.h"

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
    Ui::main_window *ui;

    QVBoxLayout* main_layout;
    QHBoxLayout* tools_layout;

    QGroupBox* preview_frame;
    QHBoxLayout* preview_layout;
    QVBoxLayout* preview_inner_layout;
    QLabel* preview_label;

    QGroupBox* tool_frame;
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
public slots:
    void slot_open_video_file();
    void slot_update_progress(int value);
    void slot_reset_slider();
    void slot_load_timeline(QPixmap* thumbnails, int cnt);
signals:
    void sig_progress_changed(double progress);
};

#endif // MAIN_WINDOW_H

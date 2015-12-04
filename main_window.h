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

    QFrame* preview_frame;
    QHBoxLayout* preview_layout;
    QVBoxLayout* preview_inner_layout;
    QLabel* preview_label;

    QFrame* tool_frame;
    QFrame* thumbnail_frame;

    QWidget* main_widget;

    QToolBar* file_toolbar;
    QToolButton* open_file_btn;

    QFileDialog* open_video_dialog;

    QThread* handler_thread;
    video_handler* handler;
public slots:
    void slot_open_video_file();
    //void slot_display_preview(QImage frame);
};

#endif // MAIN_WINDOW_H

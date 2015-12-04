#include "main_window.h"
#include "ui_main_window.h"
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
using namespace std;

main_window::main_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::main_window)
{
    this -> setFixedSize(960, 640);

    ui->setupUi(this);

    handler_thread = new QThread();
    handler = new video_handler();
    handler -> moveToThread(handler_thread);

    main_layout = new QVBoxLayout();
    tools_layout = new QHBoxLayout();

    preview_label = new QLabel();
    //preview_label -> setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //preview_label -> setFixedSize(600, 440);
    preview_layout = new QHBoxLayout();
    preview_inner_layout = new QVBoxLayout();
    preview_inner_layout -> addStretch();
    preview_inner_layout -> addWidget(preview_label);
    preview_inner_layout -> addStretch();
    preview_layout -> addStretch();
    preview_layout -> addLayout(preview_inner_layout);
    preview_layout -> addStretch();

    preview_frame = new QFrame();
    preview_frame -> setLayout(preview_layout);
    preview_frame -> setFixedSize(640, 480);

    tool_frame = new QFrame();
    thumbnail_frame = new QFrame();
    preview_frame -> setFrameStyle(QFrame::Box || QFrame::Plain);
    tool_frame -> setFrameStyle(QFrame::Box || QFrame::Plain);
    thumbnail_frame -> setFrameStyle(QFrame::Box || QFrame::Plain);
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

    open_video_dialog = new QFileDialog();

    connect(open_video_dialog, SIGNAL(fileSelected(QString)), handler, SLOT(slot_open_video(QString)));
    connect(handler, SIGNAL(sig_preview(QPixmap)), preview_label, SLOT(setPixmap(QPixmap)));

    handler_thread -> start();
}

main_window::~main_window()
{
    delete ui;
}

void main_window::slot_open_video_file()
{
    open_video_dialog -> exec();
    return;
}


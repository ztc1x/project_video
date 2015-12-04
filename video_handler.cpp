#include "video_handler.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <string>
#include <iostream>
using namespace cv;
using namespace std;

video_handler::video_handler(QObject *parent) : QObject(parent)
{
    util = new utils();
}

void video_handler::slot_open_video(QString filename)
{
    video_feed = new VideoCapture(filename.toStdString().c_str());

    if(! video_feed->isOpened())
    {
        cout << "Error: Cannot open file." << endl;
        return;
    }

    Mat frame;
    video_feed -> read(frame);
    QImage first_frame = util -> mat_2_qimage(frame);
    first_frame = first_frame.scaledToWidth(600);
    QPixmap first_frame_pixmap;
    first_frame_pixmap.convertFromImage(first_frame);
    emit sig_preview(first_frame_pixmap);

    return;
}


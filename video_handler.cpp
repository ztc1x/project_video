#include "video_handler.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QVector>
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

    if(! video_feed -> isOpened())
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

    QPixmap* thumbnails = new QPixmap[10];
    for(int i = 0; i < 10; i ++)
    {
        double progress = (double)i / 10.0;
        video_feed -> set(CV_CAP_PROP_POS_AVI_RATIO, progress);
        video_feed -> read(frame);
        QImage frame_img = util -> mat_2_qimage(frame);
        frame_img = frame_img.scaledToWidth(88);
        QPixmap tmp_pixmap;
        tmp_pixmap.convertFromImage(frame_img);
        thumbnails[i] = tmp_pixmap;
    }
    emit sig_timeline(thumbnails, 10);

    video_feed -> set(CV_CAP_PROP_POS_AVI_RATIO, 0);

    return;
}

void video_handler::slot_update_preview(double progress)
{
    if(! video_feed -> isOpened())
        return;

    video_feed -> set(CV_CAP_PROP_POS_AVI_RATIO, progress);
    Mat frame;
    video_feed -> read(frame);
    QImage preview = util -> mat_2_qimage(frame);
    preview = preview.scaledToWidth(600);
    QPixmap preview_pixmap;
    preview_pixmap.convertFromImage(preview);
    emit sig_preview(preview_pixmap);

    return;
}

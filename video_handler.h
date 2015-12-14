#ifndef VIDEO_HANDLER_H
#define VIDEO_HANDLER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QPixmap>
#include <QVector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "utils.h"

class video_handler : public QObject
{
    Q_OBJECT
private:
    cv::VideoCapture* video_feed;
    utils* util;
public:
    explicit video_handler(QObject *parent = 0);

signals:
    void sig_preview(QPixmap frame);
    void sig_preview_image(QImage image);
    void sig_timeline(QPixmap* thumbnails, int cnt);
    void sig_info(int width, int height, int fps, double duration);
    void sig_scale(int vid_w, int vid_h, int port_w, int port_h);
public slots:
    void slot_open_video(QString filename);
    void slot_update_preview(double progress);
};

#endif // VIDEO_HANDLER_H

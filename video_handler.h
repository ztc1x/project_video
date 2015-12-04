#ifndef VIDEO_HANDLER_H
#define VIDEO_HANDLER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QPixmap>
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
public slots:
    void slot_open_video(QString filename);
};

#endif // VIDEO_HANDLER_H

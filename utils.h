#ifndef UTILS_H
#define UTILS_H

#include <opencv2/core/core.hpp>
#include <QImage>

class utils
{
public:
    utils();
    QImage mat_2_qimage(cv::Mat3b mat);
};

#endif // UTILS_H

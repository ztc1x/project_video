#include "utils.h"
#include <opencv2/core/core.hpp>
#include <QImage>
using namespace cv;
using namespace std;

utils::utils()
{

}

QImage utils::mat_2_qimage(Mat3b mat)
{
    QImage ret(mat.cols, mat.rows, QImage::Format_ARGB32);
    for(int i = 0; i < mat.rows; i ++)
    {
        const cv::Vec3b* mat_row = mat[i];
        QRgb* ret_row = (QRgb*)ret.scanLine(i);
        for(int j = 0; j < mat.cols; j ++)
        {
            ret_row[j] = qRgba(mat_row[j][2], mat_row[j][1], mat_row[j][0], 255);
        }
    }
    return ret;
}

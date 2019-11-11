#ifndef __CV_QT_UTIL_H__
#define __CV_QT_UTIL_H__

//---------- opencv ---------
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtCore/QDebug>
#include "UtilDefs.h"
namespace xeyes {
	QImage UTIL_EXPORT cvMatToQImage(const cv::Mat &inMat);
	QPixmap UTIL_EXPORT cvMatToQPixmap(const cv::Mat &inMat);
	cv::Mat UTIL_EXPORT QImage2cvMat(const QImage &image);
}

#endif 


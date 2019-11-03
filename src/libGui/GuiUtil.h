#ifndef __GUI_UTIL_H__
#define __GUI_UTIL_H__

//---------- opencv ---------
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "QtHeaders.h"
#include "GuiDefs.h"
namespace xeyes {
	QImage GUI_EXPORT cvMatToQImage(const cv::Mat &inMat);
	QPixmap GUI_EXPORT cvMatToQPixmap(const cv::Mat &inMat);
	cv::Mat GUI_EXPORT QImage2cvMat(const QImage &image);

	bool GUI_EXPORT isVideoFile(const std::string &filePath);
	bool GUI_EXPORT isImgeFile(const std::string &filePath);
	void GUI_EXPORT splitExt2(const std::string& fPath, std::string& head, std::string& ext);

}

#endif 


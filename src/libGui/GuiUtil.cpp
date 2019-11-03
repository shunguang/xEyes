#include "GuiUtil.h"

using namespace std;
using namespace cv;
using namespace xeyes;

QImage  xeyes::cvMatToQImage(const cv::Mat &inMat)
{
	switch (inMat.type())
	{
		// 8-bit, 4 channel
	case CV_8UC4:
	{
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_ARGB32);

		return image;
	}

	// 8-bit, 3 channel
	case CV_8UC3:
	{
		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_RGB888);

		return image.rgbSwapped();
	}

	// 8-bit, 1 channel
	case CV_8UC1:
	{
		QImage image(inMat.cols, inMat.rows, QImage::Format_Indexed8);
		// Set the color table (used to translate colour indexes to qRgb values)  
		image.setColorCount(256);
		for (int i = 0; i < 256; i++)
		{
			image.setColor(i, qRgb(i, i, i));
		}
		// Copy input Mat  
		uchar *pSrc = inMat.data;
		for (int row = 0; row < inMat.rows; row++)
		{
			uchar *pDest = image.scanLine(row);
			memcpy(pDest, pSrc, inMat.cols);
			pSrc += inMat.step;
		}

		return image;
	}

	default:
		qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
		break;
	}

	return QImage();
}


QPixmap xeyes::cvMatToQPixmap(const cv::Mat &inMat)
{
	return QPixmap::fromImage(cvMatToQImage(inMat));
}


cv::Mat xeyes::QImage2cvMat(const QImage &image)
{
	cv::Mat mat;
	qDebug() << image.format();
	switch (image.format())
	{
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, CV_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}

bool xeyes::isVideoFile(const std::string &filePath)
{
	std::string  head, ext;

	splitExt2(filePath, head, ext);

	if (0 == ext.compare("AVI")) {
		return true;
	}
	if (0 == ext.compare("MP4")) {
		return true;
	}

	return false;
}

bool xeyes::isImgeFile(const std::string &filePath)
{
	std::string  head, ext;

	splitExt2(filePath, head, ext);

	if (0 == ext.compare("BMP")) {
		return true;
	}
	if (0 == ext.compare("JPG")) {
		return true;
	}
	if (0 == ext.compare("PNG")) {
		return true;
	}
	if (0 == ext.compare("GIF")) {
		return true;
	}
	if (0 == ext.compare("JPEG")) {
		return true;
	}
	if (0 == ext.compare("PBM")) {
		return true;
	}
	if (0 == ext.compare("PGM")) {
		return true;
	}
	if (0 == ext.compare("PPM")) {
		return true;
	}
	if (0 == ext.compare("XBM")) {
		return true;
	}
	if (0 == ext.compare("XPM")) {
		return true;
	}

	return false;
}


void xeyes::splitExt2(const std::string& fPath, std::string& head, std::string& ext)
{
	int id = fPath.find_last_of('.');
	if (id == std::string::npos) return;

	head = fPath.substr(0, id);
	ext = fPath.substr(id + 1);
}
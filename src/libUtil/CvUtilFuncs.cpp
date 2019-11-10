#include "CvUtilFuncs.h"

using namespace cv;
using namespace std;

template<typename _Tp>
_Tp xeyes::sumVal(const cv::Mat &x)
{
	_Tp s = 0;
	for (int i = 0; i<x.rows; ++i) {
		const _Tp *p = x.ptr<_Tp>(i);
		for (int j = 0; j<x.cols; ++j) {
			s += p[j];
		}
	}

	return s;
}


//return a col vector
Mat xeyes::sumCols(const cv::Mat &x)
{
	//cout<<x<<endl;
	int n = x.cols;
	Mat y = Mat::ones(n, 1, x.type());
	//cout<<x*y<<endl;
	return x*y;
}

//return a row vector
Mat xeyes::sumRows(const cv::Mat &x)
{
	//cout<<x<<endl;

	int m = x.rows;
	Mat y = Mat::ones(1, m, x.type());

	//cout<<y*x<<endl;
	return y*x;
}

//class static function
void xeyes::gray2bgr(const cv::Mat &gray, cv::Mat &bgr)
{
	vector<Mat> ch;
	ch.push_back(gray);
	ch.push_back(gray);
	ch.push_back(gray);
	merge(ch, bgr);
}

void xeyes::convert_to_CV_8U(const cv::Mat &in, cv::Mat &out)
{
	double	minVal, maxVal;
	//int		minIdx, maxIdx;

	minMaxIdx(in, &minVal, &maxVal);

	printf("minVal=%.2f, maxVal=%.2f\n", minVal, maxVal);

	double d = maxVal - minVal;
	Mat tmp;
	in.convertTo(tmp, CV_32F, 1, 0);
	if (fabs(d) > 1e-6) {
		tmp = (tmp - minVal)*(255.0 / d);
	}
	tmp.convertTo(out, CV_8U, 1, 0);
}



bool xeyes::myCvInv(const cv::Mat &X, cv::Mat &invX, double &det)
{
	det = determinant(X);
	if (det < APP_EPS2) {
		return false;
	}

	invX = X.inv();

	return true;
}

float xeyes::maskNonZeroPercent(const Mat &mask, const int &xo, const int &yo, const int &xb, const int &yb, size_t &n)
{
#if 1
	assert(xo >= 0 && xb<mask.cols && xo <= xb);
	assert(yo >= 0 && yb<mask.rows && yo <= yb);
	assert(mask.depth() == 8);
#endif

	size_t s = 0;
	uchar *col;
	for (int y = yo; y <= yb; ++y) {
		col = mask.data + mask.step[0] * y + mask.step[1] * xo;
		for (int x = xo; x <= xb; ++x) {
			if (*col++ > 0)
				s++;
		}
	}

	size_t a = (xb - xo + 1)*(yb - yo + 1);
	n = s;

	if (a>0)
		return (float)s / (float)a;
	else
		return -1.0f;
}

void xeyes::rectPyrLevelUpDown(const cv::Rect &in, const int dL, cv::Rect &out)
{
	if (dL == 0) {
		out = in;
		return;
	}

	int x(in.x), y(in.y), w(in.width), h(in.height);
	if (dL>0) {
		x = x << dL;
		y = y << dL;
		w = w << dL;
		h = h << dL;
	}
	else {
		int L = -dL;
		x = x >> L;
		y = y >> L;
		w = w >> L;
		h = h >> L;
	}

	out = cv::Rect(x, y, w, h);
}

void xeyes::setCvImg(IplImage *des, const int &desRoiXo, const int &desRoiYo,
	const IplImage *src, const int &srcRoiXo, const int &srcRoiYo, const int &srcRoiW, const int &srcRoiH)
{
#if _DEBUG
	assert(desRoiXo >= 0 && desRoiYo >= 0);
	assert(srcRoiXo >= 0 && srcRoiYo >= 0);
	assert(srcRoiXo + srcRoiW <= src->width && srcRoiYo + srcRoiH <= src->height);
	//	assert( desRoiXo+srcRoiW <= des->width && desRoiYo+srcRoiH <= src->height );
#endif

	//this version only works for 8 bits images
	assert(des->depth == IPL_DEPTH_8U);
	assert(src->depth == IPL_DEPTH_8U);

	int w = des->width;
	int h = des->height;

	int desRowStep, desRow0;
	int srcRowStep, srcRow0;

	getCvImgRowStep(des, desRowStep, desRow0);
	getCvImgRowStep(src, srcRowStep, srcRow0);

	//roi in des image
	const int &xo = desRoiXo;
	const int &yo = desRoiYo;
	int xb = xo + srcRoiW - 1;
	if (xb > w - 1) xb = w - 1;
	int yb = yo + srcRoiH - 1;
	if (yb > h - 1) yb = h - 1;

	int srcColOffset = src->nChannels * srcRoiXo * sizeof(char);
	int desColOffset = des->nChannels * desRoiXo * sizeof(char);

	unsigned char *srcRow = (unsigned char *)src->imageData + srcRowStep*srcRoiYo + srcColOffset;
	unsigned char *desRow = (unsigned char *)des->imageData + desRowStep*desRoiYo + desColOffset;
	if (src->nChannels == des->nChannels) {
		int oneRoiRowSize = src->nChannels * (xb - xo + 1) * sizeof(char);
		for (int y = yo; y <= yb; ++y, desRow += desRowStep, srcRow += srcRowStep) {
			memcpy(desRow, srcRow, oneRoiRowSize);
		}
	}
	else if (src->nChannels == 1 && des->nChannels == 3) {
		for (int y = yo; y <= yb; ++y, desRow += desRowStep, srcRow += srcRowStep) {
			unsigned char *pSrc = srcRow + xo;
			unsigned char *pDes = desRow + 3 * xo;
			for (int x = xo; x <= xb; ++x, ++pSrc) {
				*pDes++ = *pSrc;
				*pDes++ = *pSrc;
				*pDes++ = *pSrc;
			}
		}
	}
	else if (src->nChannels == 3 && des->nChannels == 1) {
		//todo
		assert(0);
	}
	else {
		assert(0);
	}
}

bool xeyes::getCvImgRowStep(const IplImage *img, int &rowStep, int &row0)
{

	if (!img) {
		rowStep = -1;
		row0 = -1;
		return false;
	}

	int h = img->height;
	row0 = 0;
	rowStep = img->widthStep;				 //assume ipl image starts from  top left

	if (img->origin == 1) {				  	 //if ipl image starts from bottom left
		row0 = (h - 1)*img->widthStep;
		rowStep = -rowStep;
	}

	return true;
}


std::string xeyes::genImgFilePath(const string &myPath, const string &fileNameWoExt, const uint64_t fn, const int L)
{
	char fileName[1024];
	snprintf(fileName, 1024, "%s/%s-%05d-L%d.png", myPath.c_str(), fileNameWoExt.c_str(), fn, L);
	return std::string(fileName);
}

void xeyes::myImgWrite(const cv::Mat &x, const std::string &dir, const std::string &fname, const size_t fn, const size_t L)
{

	std::string f = genImgFilePath( dir, fname, fn, L);
	printf("!!! Writting file: %s\n", f.c_str() );
	cv::imwrite(f, x);
}


void xeyes::myImgWrite(const IplImage* cvTrkImg, const string &myPath, const string &fileNameWoExt, const size_t &fn)
{
	std::string fileName = genImgFilePath(myPath, fileNameWoExt, fn);
	if (!folderExists(myPath)) {
		printf("xeyes::myCvImgSave(): Path: %s is not exist!\n", myPath.c_str());
		printf("xeyes::myCvImgSave(): could not save file: %s\n", fileName.c_str());
		return;
	}

	if (!cvSaveImage(fileName.c_str(), cvTrkImg)) {
		printf("xeyes::myCvImgSave():  Could not save: %s\n", fileName.c_str());
	}
	else {
		printf("xeyes::myCvImgSave():  File saved: %s\n", fileName.c_str());
	}
}

void xeyes::dumpCvImgToTextFile(const IplImage *J, const string &fileName)
{
	int w = J->width;
	int h = J->height;
	int iplRow0(0), iplRowStep(J->widthStep);  //assume ipl image starts from  top left
	if (J->origin == 1) {						//if ipl image starts from bottom left
		iplRow0 = (h - 1)*J->widthStep;
		iplRowStep = -iplRowStep;
	}

	FILE *fp = fopen(fileName.c_str(), "w");
	unsigned char *pRow = (unsigned char *)J->imageData + iplRow0;
	if (J->nChannels == 1) {
		for (int i = 0; i<h; ++i, pRow += iplRowStep) {
			unsigned char *p2 = pRow;
			for (int j = 0; j<w - 1; ++j) {
				fprintf(fp, "%d,", *p2++);
			}
			fprintf(fp, "%d\n", *p2++);
		}
	}
	else if (J->nChannels == 3) {
		for (int i = 0; i<h; ++i, pRow += iplRowStep) {
			unsigned char *p2 = pRow;
			unsigned char *pixel = p2;
			for (int j = 0; j<w; ++j, pixel += 3) {
				fprintf(fp, "(%3d,%3d,%3d)", pixel[0], pixel[1], pixel[2]);
			}
			fprintf(fp, "\n");
		}
	}
	else {
		assert(0);
	}

	fclose(fp);
}

void xeyes::drawPlus(cv::Mat &img, const cv::Point2f &pt, const uint32_t len, const cv::Scalar& color, int thickness, int lineType, int shift)
{
	Point p(pt.x, pt.y);
	drawPlus(img, p, len, color, thickness, lineType, shift);
}

void xeyes::drawPlus(Mat &img, const Point &pt, const uint32_t len, const Scalar& color, int thickness, int lineType, int shift)
{
	Point pt1(pt.x - len, pt.y);
	Point pt2(pt.x + len, pt.y);
	line(img, pt1, pt2, color, thickness, lineType, shift);

	pt1.x = pt.x; pt1.y = pt.y - len;
	pt2.x = pt.x; pt2.y = pt.y + len;
	line(img, pt1, pt2, color, thickness, lineType, shift);
}

void xeyes::drawPluses(cv::Mat &img, const std::vector<cv::Point2f> &v, const uint32_t len, const cv::Scalar& color, int thickness, int lineType, int shift)
{
	BOOST_FOREACH(const cv::Point2f &p, v){
		drawPlus( img, p, len, color, thickness, lineType, shift);
	}
}

void xeyes::drawPluses(cv::Mat &img, const std::vector<cv::Point> &v, const uint32_t len, const cv::Scalar& color, int thickness, int lineType, int shift)
{
	BOOST_FOREACH(const cv::Point &p, v) {
		drawPlus(img, p, len, color, thickness, lineType, shift);
	}
}

void xeyes::wrtieStrings(Mat& img, const vector<string>& vStrs, const cv::Point &org, int fontFace, double fontScale, cv::Scalar color, int thickness, int lineType)
{
	std::vector<std::string>::const_iterator it = vStrs.begin();
	Point pt(org);
	for (; it != vStrs.end(); ++it) {
		putText(img, *it, pt, fontFace, fontScale, color, thickness, lineType);
		pt.y += 10;
	}
}

void xeyes::writeFrmNum(cv::Mat &img, const uint64_t frmNum, const cv::Point &org )
{
	cv::putText(img, std::to_string(frmNum), org, cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0,143,143), 2);
}

void xeyes::drawRect(cv::Mat &img, const Roi &rect, const uint32_t len, const cv::Scalar& color, int thickness, int lineType, int shift)
{
	//LT
	Point p1(rect.getInt_xo(), rect.getInt_yo());
	Point p2(p1.x + len, p1.y);
	line(img, p1, p2, color, thickness, lineType, shift);
	p2.x = p1.x; p2.y = p1.y + len;
	line(img, p1, p2, color, thickness, lineType, shift);

	//RT
	p1.x = rect.getInt_xb();  //p1.y no change
	p2.x = p1.x; p2.y = p1.y + len;
	line(img, p1, p2, color, thickness, lineType, shift);
	p2.x -= len; p2.y = p1.y;
	line(img, p1, p2, color, thickness, lineType, shift);

	//RB
	//p1.x = rect.getInt_xb();  
	p1.y = rect.getInt_yb();
	p2.x = p1.x; p2.y = p1.y - len;
	line(img, p1, p2, color, thickness, lineType, shift);
	p2.x -= len; p2.y = p1.y;
	line(img, p1, p2, color, thickness, lineType, shift);

	//LB
	p1.x = rect.getInt_xo();  
	//p1.y = rect.getInt_yb();
	p2.x = p1.x; p2.y = p1.y - len;
	line(img, p1, p2, color, thickness, lineType, shift);
	p2.x += len; p2.y = p1.y;
	line(img, p1, p2, color, thickness, lineType, shift);
}

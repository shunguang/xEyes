
#include "DispFrm_h.h"

using namespace std;
using namespace cv;
using namespace xeyes;

DispFrm_h::DispFrm_h(int w, int h)
	: m_fn(0)
	, m_keyFrmIdx(0)
	, m_timeStamp_ms(0)
	//, m_img(w, h, QImage::Format_RGB888)
	, m_cvImg(w,h,CV_8UC3)
{
}


DispFrm_h::~DispFrm_h()
{
}

DispFrm_h::DispFrm_h(const DispFrm_h &x)
	: m_fn(x.m_fn)
	, m_keyFrmIdx(x.m_keyFrmIdx)
	, m_timeStamp_ms(x.m_timeStamp_ms)
	//, m_img(x.m_img)
	, m_cvImg(x.m_cvImg)
{
}

//hard copy
DispFrm_h& DispFrm_h::operator = (const DispFrm_h &x)
{
	if (this != &x) {
		m_fn = x.m_fn;
		m_keyFrmIdx = x.m_keyFrmIdx;
		m_timeStamp_ms = x.m_timeStamp_ms;
		//m_img = x.m_img;
		x.m_cvImg.copyTo( m_cvImg );
	}
	return *this;
}


void DispFrm_h::dump(const std::string &folderPath) const
{
	myImgWrite(m_cvImg, folderPath, "dispFrm", m_fn, 0);
}


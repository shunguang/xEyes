
#include "DspFrm_h.h"

using namespace std;
using namespace cv;
using namespace xeyes;

DspFrm_h::DspFrm_h(int w, int h)
	: m_fn(0)
	, m_keyFrmIdx(0)
	, m_timeStamp_ms(0)
	, m_img(w, h)
{
}


DspFrm_h::~DspFrm_h()
{
}

DspFrm_h::DspFrm_h(const DspFrm_h &x)
	: m_fn(x.m_fn)
	, m_keyFrmIdx(x.m_keyFrmIdx)
	, m_timeStamp_ms(x.m_timeStamp_ms)
	, m_img(x.m_img)
{
}

//hard copy
DspFrm_h& DspFrm_h::operator = (const DspFrm_h &x)
{
	if (this != &x) {
		m_fn = x.m_fn;
		m_keyFrmIdx = x.m_keyFrmIdx;
		m_timeStamp_ms = x.m_timeStamp_ms;
		m_img = x.m_img.copy();
	}
	return *this;
}


void DspFrm_h::dump(const std::string &folderPath) const
{
	std::string fileName = genImgFilePath(folderPath, "dspFrm", m_fn);
	m_img.save( QString::fromStdString( fileName) );
}



#include "DspFrm_h.h"

using namespace std;
using namespace cv;
using namespace xeyes;

DspFrm_h::DspFrm_h(int w, int h)
	: m_fn(0)
	, m_keyFrmIdx(0)
	, m_timeStamp_ms(0)
#if DSP_USE_QPIXMAP			
	, m_img(w, h)
#endif
{
}


DspFrm_h::~DspFrm_h()
{
}

DspFrm_h::DspFrm_h(const DspFrm_h &x)
	: m_fn(x.m_fn)
	, m_keyFrmIdx(x.m_keyFrmIdx)
	, m_timeStamp_ms(x.m_timeStamp_ms)
#if DSP_USE_QPIXMAP			
	, m_img(x.m_img)
#endif
{
}

//hard copy
DspFrm_h& DspFrm_h::operator = (const DspFrm_h &x)
{
	if (this != &x) {
		m_fn = x.m_fn;
		m_keyFrmIdx = x.m_keyFrmIdx;
		m_timeStamp_ms = x.m_timeStamp_ms;
#if DSP_USE_QPIXMAP			
		m_img = x.m_img.copy();
#endif
	}
	return *this;
}


void DspFrm_h::dump(const std::string &folderPath) const
{
	std::string fileName = genImgFilePath(folderPath, "dspFrm", m_fn);
#if DSP_USE_QPIXMAP			
	m_img.save( QString::fromStdString( fileName) );
#endif
}


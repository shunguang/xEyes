/*
*------------------------------------------------------------------------
*DspFrm_h.cpp
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
*
* THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
*
* IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTOR BE LIABLE FOR
* ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
* LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* Permission to use, copy, modify, distribute, and sell this software and
* its documentation for any purpose is prohibited unless it is granted under
* the author's written notice.
*
* Copyright(c) 2020 by Shunguang Wu, All Right Reserved
*-------------------------------------------------------------------------
*/

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


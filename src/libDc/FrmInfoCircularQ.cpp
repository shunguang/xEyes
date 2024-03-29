/*
*------------------------------------------------------------------------
*FrmInfoCircularQ.cpp
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
#include "FrmInfoCircularQ.h"

using namespace std;
using namespace cv;
using namespace xeyes;

FrmInfoCircularQ::FrmInfoCircularQ(const CfgCam &cam, const CfgLocalView &lv, const std::string &name)
	: m_items( cam.frmQueSz_ )
	, m_yuvWrtH(0)		//the index to write YuvFrms
	, m_yuvReadH(0)	    //the index to read  YuvFrms for detetcion thread
	, m_detWrtH(0)   
	, m_detReadH(0)
	, m_dspWrtH(0)
	, m_dspReadH(0)
	, m_q()
	, m_vYuvWrtCnt()
	, m_vDetWrtCnt()
	, m_vDspWrtCnt()
	, m_mutex4YuvRW()
	, m_mutex4DetRW()
	, m_mutex4DspRW()
	, m_mutex4All()
	, m_w0(cam.imgSz_.w)
	, m_h0(cam.imgSz_.h)
	, m_detPyrL(cam.detPyrLev_)
	, m_dspPyrL(lv.dispPyrLev_)
	, m_name(name)
	, m_camId(cam.cameraId_)
{
	allocQ();
}

FrmInfoCircularQ :: ~FrmInfoCircularQ()
{
	freeQ();
}

//reset size
void FrmInfoCircularQ::resetName(const std::string &name)
{
}
void FrmInfoCircularQ::resetSize(int w0, int h0, int detPyrL, int dspPyrL, const uint32_t nTotItems)
{
}

//reset <m_headW> and <m_headR> as zeros
void FrmInfoCircularQ::reset()
{
}

//wrt and read YuvFrms
bool FrmInfoCircularQ::wrtYuvFrmByCapThread(const YuvFrm_h *src)  	//host interface
{
	static int wrtDropCnt = 0;
	bool sucWrt = false;
	{
		boost::mutex::scoped_lock lock(m_mutex4YuvRW);
		uint32_t &idx = m_yuvWrtH;
		int   &cnt = m_vYuvWrtCnt[idx];
		if (cnt == 0) {
			FrmInfo  *dst = m_q[idx].get();
			dst->yuv_.hdCopyFrom(src);	  //hard copy
			cnt = cnt + 1;

			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
			sucWrt = true;
		}
	}

	if (!sucWrt) {
		++wrtDropCnt;
		if (wrtDropCnt > 999) {
			dumpLog("FrmInfoCircularQ::wrtNext(): writen is too fast, %d frames droped", wrtDropCnt);
			wrtDropCnt = 0;
		}
	}
	return sucWrt;
}

//assumption: only detection therad calls this function once for each frm
bool FrmInfoCircularQ::readYuvFrmByDetThread(YuvFrm_h *dst)       	//host interface 
{
	bool hasData = false;
	{
		//this lock moved to readNext() for specific RawFrmXYZ Type
		boost::mutex::scoped_lock lock(m_mutex4YuvRW);
		uint32_t &idx = m_yuvReadH;
		int   &cnt = m_vYuvWrtCnt[idx];
		if (cnt > 0) {
			FrmInfo  *src = m_q[idx].get();
			src->yuv_.hdCopyTo(dst);		//hd copy the poiter
			cnt = 0;                        
			hasData = true;

			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
		}
	}
	return hasData;

}

#if APP_USE_CUDA
bool FrmInfoCircularQ::wrtYuvFrmByCapThread(const YuvFrm_d *src)  	//dev interface
{
}
bool FrmInfoCircularQ::readYuvFrmByDetThread(YuvFrm_d* dst)		//dev interface
{
}
#endif
//wrt and read DetFrms
bool FrmInfoCircularQ::wrtDetFrmByDetThread(const DetFrm_h *src)  	//host interface
{
	static int wrtDropCnt = 0;
	bool sucWrt = false;
	{
		boost::mutex::scoped_lock lock(m_mutex4DetRW);
		uint32_t &idx = m_detWrtH;
		int   &cnt = m_vDetWrtCnt[idx];
		if (cnt == 0) {
			FrmInfo  *dst = m_q[idx].get();
			dst->det_.hdCopyFrom(src);
			cnt = cnt + 1;

			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
			sucWrt = true;
		}
	}

	if (!sucWrt) {
		++wrtDropCnt;
		if (wrtDropCnt > 999) {
			dumpLog("FrmInfoCircularQ::wrtDetFrmByDetThread(): writen is too fast, %d frames droped", wrtDropCnt);
			wrtDropCnt = 0;
		}
	}
	return sucWrt;
}

//assumption: only display thread calls this function once for each frm
bool FrmInfoCircularQ::readDetFrmByDspThread(DetFrm_h *dstDet)		//host interface 
{
	bool hasData = false;
	{
		//this lock moved to readNext() for specific RawFrmXYZ Type
		boost::mutex::scoped_lock lock(m_mutex4DetRW);
		uint32_t &idx = m_detReadH;
		int   &cnt = m_vDetWrtCnt[idx];
		if (cnt > 0) {
			FrmInfo  *src = m_q[idx].get();
			src->det_.hdCopyTo(dstDet);		//hd copy the poiter
			cnt = 0;
			hasData = true;
			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
		}
	}
	return hasData;
}

//wrt and read DspFrms
bool FrmInfoCircularQ::wrtDspFrmByDspThread(const DspFrm_h *src)  	//host interface
{
	static int wrtDropCnt = 0;
	bool sucWrt = false;
	{
		boost::mutex::scoped_lock lock(m_mutex4DspRW);
		uint32_t &idx = m_dspWrtH;
		int   &cnt = m_vDspWrtCnt[idx];
		if (cnt == 0) {
			FrmInfo  *dst = m_q[idx].get();
			dst->dsp_.hdCopyFrom(src);
			cnt = cnt + 1;

			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
			sucWrt = true;
		}
	}

	if (!sucWrt) {
		++wrtDropCnt;
		if (wrtDropCnt > 999) {
			dumpLog("FrmInfoCircularQ::wrtDspFrmByDspThread(): writen is too fast, %d frames droped", wrtDropCnt);
			wrtDropCnt = 0;
		}
	}
	return sucWrt;
}

bool FrmInfoCircularQ::readDspFrmByGuiThread(DspFrm_h *dst)       //host interface 
{
	bool hasData = false;
	{
		//this lock moved to readNext() for specific RawFrmXYZ Type
		boost::mutex::scoped_lock lock(m_mutex4DspRW);
		uint32_t &idx = m_dspReadH;
		int   &cnt = m_vDspWrtCnt[idx];
		if (cnt > 0) {
			FrmInfo  *src = m_q[idx].get();
			src->dsp_.hdCopyTo(dst);		//hd copy the poiter
			cnt = 0;
			hasData = true;

			//move head to the next slot
			++idx;
			if (idx >= m_items) {
				idx = 0;
			}
		}
	}
	return hasData;
}

void FrmInfoCircularQ::freeQ()
{
	boost::mutex::scoped_lock lock(m_mutex4All);
	m_q.clear();
	m_vYuvWrtCnt.clear();
	m_vDetWrtCnt.clear();
	m_vDspWrtCnt.clear();
}

void FrmInfoCircularQ::allocQ()
{

	dumpLog("FrmInfoCircularQ::allocQ(),  m_name=%s,  m_items=%d, m_w0=%d, m_h0=%d, m_detPyrL=%d, m_dspPyrL=%d", 
		m_name.c_str(), m_items, m_w0, m_h0, m_detPyrL, m_dspPyrL );

	boost::mutex::scoped_lock lock(m_mutex4All);
	m_q.clear();
	for (uint32_t i = 0; i < m_items; ++i) {
		FrmInfoPtr p(new FrmInfo(m_w0, m_h0, m_detPyrL, m_dspPyrL, i) );
		m_q.push_back(p);
	}
	
	m_vYuvWrtCnt.resize(m_items, 0); //init YuvWrtFlags as zeros
	m_vDetWrtCnt.resize(m_items, 0);
	m_vDspWrtCnt.resize(m_items, 0);

	m_yuvWrtH = 0;		//the index to write YuvFrms
	m_yuvReadH = 0;	//the index to read  YuvFrms for detetcion thread
	m_detWrtH	= 0;	//the index to write DetFrms
	m_detReadH  = 0;	//the index to read  DetFrms
	m_dspWrtH	= 0;	//the index to write DspFrms
	m_dspReadH  = 0;	//the index to read  DspFrms

	dumpLog("FrmInfoCircularQ::allocQ done!");
}


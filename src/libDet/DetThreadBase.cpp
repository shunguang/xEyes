/*
*------------------------------------------------------------------------
*DetThreadBase.cpp
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
#include "libDsp/DspThread.h"
#include "libyuv/scale.h"
#include "DetThreadBase.h"
using namespace std;
using namespace xeyes;
DetThreadBase::DetThreadBase( const int camId, const int threadId, const std::string &threadName )
: ThreadX	( THD_TASK_DET, threadId, threadName )
, m_frmNum(0)
, m_camId 	( camId )
, m_detPyrL(1)
, m_detMethod(DET_MTD_DEEP_NETWORK)
, m_detNetworkName("ped")
, m_detFrmsToSkip(5)
, m_yuvFrm_h(0)
, m_yuvFrmAtDetSz_h(0)
, m_detFrm_h(0)
, m_camDc(0)
, m_dspPtr(0)
, m_skipedFrmCount(0)
{
	cout << "DetThreadBase::DetThreadBase(): called" << endl;
}

DetThreadBase::~DetThreadBase()
{
}

void DetThreadBase::setDspPtr(DspThread *p)
{
	m_dspPtr = p;
}

//prepare m_detFrm_h->m_rgbImg
void DetThreadBase::prepareDetImg()
{
	const int &w0 = m_yuvFrm_h->w_;
	const int &h0 = m_yuvFrm_h->h_;
	const int &w = m_yuvFrmAtDetSz_h->w_;
	const int &h = m_yuvFrmAtDetSz_h->h_;
	//dumpLog("A--w=%d,h=%d,w2=%d,h2=%d",w,h, m_detFrm_h->m_rgbImg.I_.cols, m_detFrm_h->m_rgbImg.I_.rows );
	myAssert(  w == m_detFrm_h->m_rgbImg.I_.cols && h == m_detFrm_h->m_rgbImg.I_.rows, "DetThreadBase::prepareRgb4Det(): img sz does not match!" );

	if ( w !=w0 || h !=h0 ){
		int ret = libyuv::I420Scale(
			m_yuvFrm_h->pBuf_[0],  w0,
            m_yuvFrm_h->pBuf_[1],  w0/2,
            m_yuvFrm_h->pBuf_[2],  w0/2,
            w0,  h0,
			m_yuvFrmAtDetSz_h->pBuf_[0],  w,
            m_yuvFrmAtDetSz_h->pBuf_[1],  w/2,
            m_yuvFrmAtDetSz_h->pBuf_[2],  w/2,
            w,   h,
            libyuv::kFilterBilinear /*kFilterNone, kFilterLinear, kFilterBilinear, kFilterBox*/
		  );

		m_yuvFrmAtDetSz_h->hdCopyToBGR( &(m_detFrm_h->m_rgbImg.I_) );
	}
	else{
		m_yuvFrm_h->hdCopyToBGR( &(m_detFrm_h->m_rgbImg.I_) );
	}
	m_detFrm_h->m_rgbImg.fn_ = m_yuvFrm_h->fn_;
	m_detFrm_h->m_fn = m_yuvFrm_h->fn_;
	m_detFrm_h->m_L = m_detPyrL;
}

void DetThreadBase::prepareOutputImg()
{
	cv::Mat &I = m_detFrm_h->m_rgbImg.I_;
	for ( auto &x : m_detFrm_h->m_vRois) {
		x.trimLocation(I.cols, I.rows);
		cv::Rect rect = x.toCvRect();
		//cout << "rect=" << rect <<endl;
		cv::rectangle(I, rect, cv::Scalar(0, 255, 0), 2);
	}
	cv::putText( I, std::to_string(m_detFrm_h->m_fn), cv::Point(10, I.rows-20), cv::FONT_HERSHEY_DUPLEX, 2, cv::Scalar(0, 255, 255), 2);
	//cout << "fn=" << m_detFrm_h->m_fn <<endl;
}

void DetThreadBase::dumpFrm( const std::string &folderPath )
{
	m_detFrm_h->dump( folderPath, "detFrm" );
}

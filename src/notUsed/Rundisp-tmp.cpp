/*
*------------------------------------------------------------------------
*Rundisp-tmp.cpp
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
#include "RunDetBkgChg.h"

using namespace std;
using namespace cv;
using namespace xeyes;
RunDetBkgChg::RunDetBkgChg( const int camId, const int threadId, const std::string &threadName )
: RunDetBase( camId, threadId, threadName )
{
	cout << "RunDetBkgChg::RunDetBkgChg(): called" << endl;
}

RunDetBkgChg::~RunDetBkgChg()
{
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void RunDetBkgChg::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//read new frm from capture Q
	bool hasNewFrm = m_camDc->m_detYuvFrmQ.read( &m_yuvFrm_h ); 
	if(hasNewFrm){
		this->goToSleep();
	}

	//do change detection
	bool suc = doChgDet();

	//wrt results into output queue
	m_camDc->m_detRgbFrmQ.wrt( m_rgbFrm_h.get() );

	//decide if need to sleep
	uint32_t dt = timeIntervalMillisec(start);
	if (m_yuvFrm_h.fn_ % m_frmFreqToLog == 0) {
		dumpLog( "RunDetBkgChg::procNextTask(): %s, fn=%llu, dt=%d", m_threadName.c_str(), m_yuvFrm_h.fn_, dt);
	}

}

bool RunDetBkgChg::doChgDet()
{
	cv::Mat I0;
	m_yuvFrm_h.hdCopyToBGR( &I0 );

	int w = I0.cols >> m_detPyrL;
	int h = I0.rows >> m_detPyrL;

	cv::resize( I0, m_rgbFrm_h.I, cv::Size(w, h) );
	m_rgbFrm_h.fn_ = m_yuvFrm_h.fn_;
	m_rgbFrm_h.L_ = m_detPyrL;
}

bool RunDetBkgChg::procInit()
{
	//init this camera
	m_camDc = m_dcUI->m_dcMap[m_camId];
	CfgCam camCfg = m_cfg->getCam( m_camId );
	const int &w0 = camCfg.imgSz_.w;
	const int &h0 = camCfg.imgSz_.h;
	const int &L  = camCfg.detPyrLev_;
	
	//init currrent camera capture params
	m_detPyrL = L;
	m_camName = camCfg.cameraName_;
	m_yuvFrm_h.reset( new Yuv420Frm_h( w0, h0 ) );
	m_rgbFrm_h.reset( new RgbFrm_h( w0>>L, h0>>L, L) );

	return true;
}

/*
*------------------------------------------------------------------------
*CapThreadSyn.cpp
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
#include "libDet/DetThreadBase.h"
#include "CapThreadSyn.h"

using namespace std;
using namespace xeyes;
CapThreadSyn::CapThreadSyn( const int camId, const int threadId, const std::string &threadName )
: CapThreadBase( camId, threadId, threadName )
{
	cout << "CapThreadSyn::CapThreadSyn(): called" << endl;
}

CapThreadSyn::~CapThreadSyn()
{
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void CapThreadSyn::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//write frm number on img
	cv::Mat currFrm;
	m_synBgrFrm.copyTo(currFrm);
	cv::putText(currFrm, std::to_string(m_frmNum), cv::Point(currFrm.cols/2, currFrm.rows/2), cv::FONT_HERSHEY_DUPLEX, 4, cv::Scalar(0,143,143), 2);
	m_yuvFrm_h->hdCopyFromBGR( &currFrm,  m_frmNum);

	//wrt into queue
	m_camDc->m_frmInfoQ->wrtYuvFrmByCapThread( m_yuvFrm_h.get() );
	if (m_detPtr){
		m_detPtr->wakeupToWork();
	}
	
	//decide if need to sleep
	uint32_t dt = timeIntervalMillisec(start);
	if (dt < m_frmInterval_ms) {
		THREAD_SLEEP(m_frmInterval_ms - dt);
	}
	if (m_frmNum % m_frmFreqToLog == 0) {
		dumpLog( "CapThreadSyn::procNextTask(): %s, fn=%llu, dt=%d", m_threadName.c_str(), m_frmNum, dt);
	}

	//---- for next frm ------------
	m_frmNum++;
}

bool CapThreadSyn::procInit()
{
	//init this camera
	m_camDc = m_dcUI->m_dcMap[m_camId];
	m_camCfg = m_cfg->getCam( m_camId );
	const int &w = m_camCfg.imgSz_.w;
	const int &h = m_camCfg.imgSz_.h;
	
	//init currrent camera capture params
	m_frmInterval_ms = floor( 1000.0/m_camCfg.fps_.getFps() );
	m_yuvFrm_h.reset( new YuvFrm_h( w, h ) );
	//dumpLog( "Yuv: (w=%d,h=%d)", m_yuvFrm_h->w_, m_yuvFrm_h->h_);

	//read the synthetic frame
	
#if _WINDOWS
	string f = "C:/Projects/xEyes/data/camSyn" + std::to_string( m_camId ) + ".jpg";
#else
	string f = "../data/camSyn" + std::to_string(m_camId) + ".jpg";
#endif

	if ( !boost::filesystem::exists(boost::filesystem::path(f))) {
		myExit("CapThreadSyn::procInit(): file <%s> does not exist!", f);
		return false;
	}
	cv::Mat I = cv::imread( f, CV_LOAD_IMAGE_COLOR );
	cv::resize( I, m_synBgrFrm, cv::Size(w, h) );
	//dumpLog( "BGR: (w=%d,h=%d)", m_synBgrFrm.cols, m_synBgrFrm.rows );

	return true;
}

#include "libDsp/DspThread.h"
#include "DetThreadBkgChg.h"

using namespace std;
using namespace cv;
using namespace xeyes;
DetThreadBkgChg::DetThreadBkgChg( const int camId, const int threadId, const std::string &threadName )
: DetThreadBase( camId, threadId, threadName )
{
	cout << "DetThreadBkgChg::DetThreadBkgChg(): called" << endl;
}

DetThreadBkgChg::~DetThreadBkgChg()
{
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void DetThreadBkgChg::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//read new frm from capture Q
	bool hasNewFrm = m_camDc->m_frmInfoQ->readYuvFrmByDetThread( m_yuvFrm_h.get() );
	if(hasNewFrm){
		this->goToSleep();
		//dumpLog("DetThreadBkgChg():%s, go to sleep", m_threadName.c_str());
	}

	//do change detection
	bool suc = doChgDet();

	//wrt results into output queue
	m_camDc->m_frmInfoQ->wrtDetFrmByDetThread( m_detFrm_h.get() );
	m_dspPtr->wakeupToWork();

	if (m_yuvFrm_h->fn_ % m_frmFreqToLog == 0) {
		uint32_t dt = timeIntervalMillisec(start);
		dumpLog( "DetThreadBkgChg::procNextTask(): %s, fn=%lld, dt=%d", m_threadName.c_str(), m_yuvFrm_h->fn_, dt);
	}

}

bool DetThreadBkgChg::doChgDet()
{
	cv::Mat I0;
	m_yuvFrm_h->hdCopyToBGR( &I0 );
	int w = I0.cols >> m_detPyrL;
	int h = I0.rows >> m_detPyrL;
	//down sizing the original image

	//do detection

	//prepare results
	m_detFrm_h->m_vRois.clear();
	m_detFrm_h->m_fn = m_yuvFrm_h->fn_;
	m_detFrm_h->m_L = m_detPyrL;
	if ( 0==m_detFrm_h->m_fn % 200) {
		m_detFrm_h->m_vRois.push_back(Roi(10, 10, 50, 70));
	}
	return true;
}

bool DetThreadBkgChg::procInit()
{
	//init this camera
	m_camDc = m_dcUI->m_dcMap[m_camId];
	CfgCam camCfg = m_cfg->getCam( m_camId );
	const int &w0 = camCfg.imgSz_.w;
	const int &h0 = camCfg.imgSz_.h;
	const int &L  = camCfg.detPyrLev_;
	
	//init currrent camera capture params
	m_detPyrL = L;
	m_yuvFrm_h.reset( new YuvFrm_h( w0, h0 ) );
	m_detFrm_h.reset( new DetFrm_h( L ) );

	return true;
}

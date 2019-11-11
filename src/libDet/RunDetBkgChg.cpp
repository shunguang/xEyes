#include "libDsp/RunDsp.h"
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
	bool hasNewFrm = m_camDc->m_frmInfoQ->readYuv( m_yuvFrm_h.get(), READ_YUV_4_DET);
	if(hasNewFrm){
		this->goToSleep();
	}

	//do change detection
	bool suc = doChgDet();

	//wrt results into output queue
	m_camDc->m_frmInfoQ->wrtDet( m_detFrm_h.get() );
	m_dspPtr->wakeupToWork();

	if (m_yuvFrm_h->fn_ % m_frmFreqToLog == 0) {
		uint32_t dt = timeIntervalMillisec(start);
		dumpLog( "RunDetBkgChg::procNextTask(): %s, fn=%ulld, dt=%d", m_threadName.c_str(), m_yuvFrm_h->fn_, dt);
	}

}

bool RunDetBkgChg::doChgDet()
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
	if (m_detFrm_h->m_fn % 200) {
		m_detFrm_h->m_vRois.push_back(Roi(10, 10, 50, 70));
	}
	return true;
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
	m_yuvFrm_h.reset( new YuvFrm_h( w0, h0 ) );
	m_detFrm_h.reset( new DetFrm_h( L ) );

	return true;
}

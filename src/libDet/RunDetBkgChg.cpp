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
	bool hasNewFrm = m_camDc->m_capYuvFrmQ.read( m_yuvFrm_h.get() ); 
	if(hasNewFrm){
		this->goToSleep();
	}

	//do change detection
	bool suc = doChgDet();

	//wrt results into output queue
	m_camDc->m_detRgbFrmQ.wrt( m_rgbFrm_h.get() );

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

	cv::resize( I0, m_rgbFrm_h->I_, cv::Size(w, h) );
	m_rgbFrm_h->fn_ = m_yuvFrm_h->fn_;
	m_rgbFrm_h->L_ = m_detPyrL;
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
	m_camName = camCfg.cameraName_;
	m_yuvFrm_h.reset( new Yuv420Frm_h( w0, h0 ) );
	m_rgbFrm_h.reset( new RgbFrm_h( w0>>L, h0>>L, L) );

	return true;
}

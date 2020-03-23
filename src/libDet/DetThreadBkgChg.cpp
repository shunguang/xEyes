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
	//dumpLog("DetThreadBkgChg():AA--%s,w=%d,h=%d,sz=%d", m_threadName.c_str(), m_yuvFrm_h->w_, m_yuvFrm_h->h_, m_yuvFrm_h->sz_);

	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//read new frm from capture Q
	bool hasNewFrm = m_camDc->m_frmInfoQ->readYuvFrmByDetThread( m_yuvFrm_h.get() );
	//dumpLog("DetThreadBkgChg():BB--%s,w=%d,h=%d,sz=%d", m_threadName.c_str(), m_yuvFrm_h->w_, m_yuvFrm_h->h_, m_yuvFrm_h->sz_);
	if( !hasNewFrm ){
		this->goToSleep();
		//dumpLog("DetThreadBkgChg():%s, go to sleep", m_threadName.c_str());
		return;
	}

	//do change detection and prepare data in <m_detFrm_h>
	bool suc = doChgDet();

	//wrt results into output queue
	m_camDc->m_frmInfoQ->wrtDetFrmByDetThread( m_detFrm_h.get() );
	if( m_dspPtr){
		m_dspPtr->wakeupToWork();
	}
	else{
		//for debug w/o <m_dspPtr>: we read data from que, otherwise the que will be overflowed
		DetFrm_h tmp;
		bool hasDetFrm = m_camDc->m_frmInfoQ->readDetFrmByDspThread( &tmp );
		if( hasDetFrm  && tmp.m_fn%m_frmFreqToLog==0){
			tmp.dump(".", "detFrm");
		}
	}
	if (m_yuvFrm_h->fn_ % m_frmFreqToLog == 0) {
		uint32_t dt = timeIntervalMillisec(start);
		dumpLog( "DetThreadBkgChg::procNextTask(): %s, fn=%llu, dt=%d", m_threadName.c_str(), m_yuvFrm_h->fn_, dt);
	}

}

bool DetThreadBkgChg::doChgDet()
{
	//yuv-> resizedYuv -> rgb
	prepareDetImg();
	//do detection

	//prepare results
	m_detFrm_h->m_vRois.clear();
	if ( 0==m_detFrm_h->m_fn % 20) {
		m_detFrm_h->m_vRois.push_back(Roi(10, 10, 50, 70));
	}
	prepareOutputImg();
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
	m_yuvFrmAtDetSz_h.reset( new YuvFrm_h( w0>>L, h0>>L ) );	
	m_detFrm_h.reset( new DetFrm_h( w0, h0, L ) );

	dumpLog( "DetThreadBkgChg::procInit():called!" );

	return true;
}

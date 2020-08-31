#include "RunDisp.h"

using namespace std;
using namespace xeyes;
RunDisp::RunDisp( const int threadId, const std::string &threadName )
: ThreadX	( THD_TASK_DSP, threadId, threadName )
, m_frmNum(0)
, m_DspFrm_h(0)
{
	cout << "RunDisp::RunDisp(): called" << endl;
}

RunDisp::~RunDisp()
{
}

void RunDisp::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	bool hasAny =false;	
	for ( const auto &id : m_vCamId ){
		bool hasNewFrm = m_dcUI->m_dcMap[id]->m_detRgbFrmQ.read( m_camFrm_h.get() ); 		
		if (hasNewFrm){
			dumpLog( "camId=%d has a new frm", id);
			m_DspFrm_h->fn_ = m_camFrm_h->fn_;
		}
		hasAny = hasAny || hasNewFrm;
	}

	//wrt results into dispFrm queue
	if (hasAny){
		m_dcUI->m_dispRgbFrmQ.wrt( m_DspFrm_h.get() );
	}

	//decide if need to sleep
	if (m_DspFrm_h->fn_ % m_frmFreqToLog == 0) {
		uint32_t dt = timeIntervalMillisec(start);
		dumpLog( "RunDisp::procNextTask(): %s, fn=%llu, dt=%d", m_threadName.c_str(), m_DspFrm_h->fn_, dt);
	}

	if (!hasAny){
		this->goToSleep();
	}
}

bool RunDisp::procInit()
{
	m_cfg->getCamIds( m_vCamId );

	m_lvCfg = m_cfg->getLocalView();
	ImgSize sz = m_lvCfg.getDispImgSz();
	//init currrent camera capture params
	m_DspFrm_h.reset( new RgbFrm_h( sz.w, sz.h ) );

	int camId =  m_vCamId[0];
	CfgCam camCfg = m_cfg->getCam( camId );
	const int &w0 = camCfg.imgSz_.w;
	const int &h0 = camCfg.imgSz_.h;
	
	//init currrent camera capture params
	int L = m_lvCfg.dispPyrLev_;
	m_camFrm_h.reset( new RgbFrm_h( w0>>L, h0>>L, L) );

	return true;
}

void RunDisp::dumpDispFrm( const std::string &folderPath )
{
	m_DspFrm_h->dump( folderPath, "dispFrm" );
}

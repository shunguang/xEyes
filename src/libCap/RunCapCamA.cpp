#include "libDet/RunDetBase.h"
#include "RunCapCamA.h"

using namespace std;
using namespace xeyes;
RunCapCamA::RunCapCamA( const int camId, const int threadId, const std::string &threadName )
: RunCapBase( camId, threadId, threadName )
{
	cout << "RunCapCamA::RunCapCamA(): called" << endl;
}

RunCapCamA::~RunCapCamA()
{
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void RunCapCamA::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//write frm number on img
	cv::Mat currFrm( m_synBgrFrm);
	cv::putText(currFrm, std::to_string(m_frmNum), cv::Point(50, 50), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0,143,143), 2);
	m_yuvFrm_h->hdCopyFromBGR( &currFrm,  m_frmNum);

	//wrt into queue
	m_camDc->m_capYuvFrmQ.wrt( m_yuvFrm_h.get());

	m_detPtr->wakeupToWork();

	//decide if need to sleep
	uint32_t dt = timeIntervalMillisec(start);
	if (dt < m_frmInterval_ms) {
		THREAD_SLEEP(m_frmInterval_ms - dt);
	}
	if (m_frmNum % m_frmFreqToLog == 0) {
		dumpLog( "RunCapCamA::procNextTask(): %s, fn=%ulld, dt=%d", m_threadName.c_str(), m_frmNum, dt);
	}

	//---- for next frm ------------
	m_frmNum++;
}

bool RunCapCamA::procInit()
{
	//init this camera
	m_camDc = m_dcUI->m_dcMap[m_camId];
	m_camCfg = m_cfg->getCam( m_camId );
	const int &w = m_camCfg.imgSz_.w;
	const int &h = m_camCfg.imgSz_.h;
	
	//init currrent camera capture params
	m_frmInterval_ms = floor( 1000.0/m_camCfg.fps_.getFps() );
	m_camName = m_camCfg.cameraName_;
	m_yuvFrm_h.reset( new Yuv420Frm_h( w, h ) );
	//dumpLog( "Yuv: (w=%d,h=%d)", m_yuvFrm_h->w_, m_yuvFrm_h->h_);

	//read the synthetic frame
	const char *f = "../data/camSyncA.jpg";
	if ( !boost::filesystem::exists(boost::filesystem::path(f))) {
		myExit("RunCapCamA::procInit(): file <%s> does not exist!", f);
		return false;
	}
	cv::Mat I = cv::imread( f, CV_LOAD_IMAGE_COLOR );
	cv::resize( I, m_synBgrFrm, cv::Size(w, h) );
	//dumpLog( "BGR: (w=%d,h=%d)", m_synBgrFrm.cols, m_synBgrFrm.rows );

	return true;
}

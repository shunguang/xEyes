#include "DspThread.h"

using namespace std;
using namespace xeyes;
DspThread::DspThread( const int camId, const int threadId, const std::string &threadName )
: ThreadQt	( THD_TASK_DET, threadId, threadName )
, m_camId 	( camId )
, m_detPyrL(0)
, m_dspPyrL(0)
, m_dspSz(320,240)
, m_yuvFrm_h(0)
, m_detFrm_h(0)
, m_dspFrm_h(0)
, m_camDc(0)
, m_dspImg(320,240, CV_8UC3)
{
	cout << "DspThread::DspThread(): called" << endl;
}

DspThread::~DspThread()
{
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void DspThread::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//read new frm from capture Q
	bool hasDetFrm = m_camDc->m_frmInfoQ->readFrmsByDspThread( m_detFrm_h.get(), m_yuvFrm_h.get() );
	if (!hasDetFrm) {
		this->goToSleep();
		//dumpLog("DspThread():%s, go to sleep", m_threadName.c_str());
	}

	//do change detection
	bool suc = prepareDspImg();

	//wrt results into output queue
	m_camDc->m_frmInfoQ->wrtDspFrmByDspThread(m_dspFrm_h.get());

	emit sigReady2Disp();

	if (m_yuvFrm_h->fn_ % m_frmFreqToLog == 0) {
		uint32_t dt = timeIntervalMillisec(start);
		dumpLog("DspThread::procNextTask(): %s, fn=%lld, dt=%d", m_threadName.c_str(), m_yuvFrm_h->fn_, dt);
	}

}

bool DspThread::prepareDspImg()
{
	cv::Mat I0;
	m_yuvFrm_h->hdCopyToBGR(&I0);

	//down sizing the original image
	cv::resize(I0, m_dspImg, m_dspSz, 0, 0, CV_INTER_LINEAR);

	//draw ROis
	int dL = m_detPyrL - m_dspPyrL;
	for (const auto &x : m_detFrm_h->m_vRois) {
		Roi y(x);
		if (dL > 0) {
			for (int i = 0; i < dL; ++i) {
				y.oneLevelUp();
			}
		}
		else if (dL < 0) {
			for (int i = 0; i < -dL; ++i) {
				y.oneLevelDown();
			}
		}
		cv::Rect rect = y.toCvRect();
		cv::rectangle(m_dspImg, rect, cv::Scalar(0, 255, 0), 2);
	}

	cv::putText(m_dspImg, std::to_string(m_yuvFrm_h->fn_), cv::Point(50, m_dspSz.height-50), cv::FONT_HERSHEY_DUPLEX, 2, cv::Scalar(255, 255, 255), 2);

	//convert to QBitmap
#if DSP_USE_QPIXMAP		
	m_dspFrm_h->m_img = cvMatToQPixmap(m_dspImg);
#else
	myExit("DspThread::prepareDspImg():  todo!");
#endif	
	m_dspFrm_h->m_fn = m_yuvFrm_h->fn_;
	return true;
}

bool DspThread::procInit()
{
	m_camDc = m_dcUI->m_dcMap[m_camId];

	CfgCam camCfg = m_cfg->getCam(m_camId);
	const int &w0 = camCfg.imgSz_.w;
	const int &h0 = camCfg.imgSz_.h;
	m_detPyrL = camCfg.detPyrLev_;

	m_dspSz = m_cfg->getDspImgSz( m_camId );

	m_yuvFrm_h.reset(new YuvFrm_h(w0, h0));
	m_detFrm_h.reset(new DetFrm_h(m_detPyrL));
	m_dspFrm_h.reset(new DspFrm_h(m_dspSz.width, m_dspSz.height));
	m_dspImg.create(m_dspSz, 0);
	return true;
}

void DspThread::dumpFrm(const std::string &folderPath)
{
	m_dspFrm_h->dump(folderPath);
}

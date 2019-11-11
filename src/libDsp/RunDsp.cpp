#include "RunDsp.h"

using namespace std;
using namespace xeyes;
RunDsp::RunDsp( const int camId, const int threadId, const std::string &threadName )
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
	cout << "RunDsp::RunDsp(): called" << endl;
}

RunDsp::~RunDsp()
{
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void RunDsp::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//read new frm from capture Q
	bool hasCapFrm = m_camDc->m_frmInfoQ->readYuv(m_yuvFrm_h.get(), READ_YUV_4_DSP);
	bool hasDetFrm = m_camDc->m_frmInfoQ->readDet(m_detFrm_h.get());
	if (!hasCapFrm && !hasDetFrm) {
		this->goToSleep();
	}

	//do change detection
	bool suc = prepareDspImg();

	//wrt results into output queue
	m_camDc->m_frmInfoQ->wrtDsp(m_dspFrm_h.get());

	emit sigReady2Disp(m_dspFrm_h->m_fn);

	if (m_yuvFrm_h->fn_ % m_frmFreqToLog == 0) {
		uint32_t dt = timeIntervalMillisec(start);
		dumpLog("RunDsp::procNextTask(): %s, fn=%ulld, dt=%d", m_threadName.c_str(), m_yuvFrm_h->fn_, dt);
	}

}

bool RunDsp::prepareDspImg()
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

	//convert to QBitmap
	m_dspFrm_h->m_img = cvMatToQPixmap(m_dspImg);
	m_dspFrm_h->m_fn = m_yuvFrm_h->fn_;
	return true;
}

bool RunDsp::procInit()
{
	//init this camera
	m_camDc = m_dcUI->m_dcMap[m_camId];
	CfgCam camCfg = m_cfg->getCam(m_camId);
	const int &w0 = camCfg.imgSz_.w;
	const int &h0 = camCfg.imgSz_.h;
	m_detPyrL = camCfg.detPyrLev_;
	CfgLocalView lv = m_cfg->getLocalView();

	//init currrent camera capture params
	m_dspPyrL = lv.dispPyrLev_;
	m_dspSz = cv::Size(w0 >> m_dspPyrL, h0 >> m_dspPyrL);


	m_yuvFrm_h.reset(new YuvFrm_h(w0, h0));
	m_detFrm_h.reset(new DetFrm_h(m_detPyrL));
	m_dspFrm_h.reset(new DspFrm_h(m_dspSz.width, m_dspSz.height));
	m_dspImg.create(m_dspSz, 0);
	return true;
}

void RunDsp::dumpFrm(const std::string &folderPath)
{
	m_dspFrm_h->dump(folderPath);
}

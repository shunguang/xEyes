#include "libDsp/DspThread.h"
#include "DetThreadDeepNet.h"

using namespace std;
using namespace cv;
using namespace xeyes;
DetThreadDeepNet::DetThreadDeepNet( const int camId, const int threadId, const std::string &threadName )
: DetThreadBase( camId, threadId, threadName )
, m_deepNet(0)
, m_overlayFlags(0)
, m_detImgCpu (NULL)
, m_detImgGpu (NULL)
, m_detImgW(0)
, m_detImgH(0)
{
	cout << "DetThreadDeepNet::DetThreadDeepNet(): called" << endl;
}

DetThreadDeepNet::~DetThreadDeepNet()
{
	if (m_detImgCpu) {
		CUDA(cudaFreeHost(m_detImgCpu));
	}
	DetThreadBase::~DetThreadBase()
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void DetThreadDeepNet::procNextTask()
{
	//dumpLog("DetThreadDeepNet():AA--%s,w=%d,h=%d,sz=%d", m_threadName.c_str(), m_yuvFrm_h->w_, m_yuvFrm_h->h_, m_yuvFrm_h->sz_);

	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//read new frm from capture Q
	bool hasNewFrm = m_camDc->m_frmInfoQ->readYuvFrmByDetThread( m_yuvFrm_h.get() );
	//dumpLog("DetThreadDeepNet():BB--%s,w=%d,h=%d,sz=%d", m_threadName.c_str(), m_yuvFrm_h->w_, m_yuvFrm_h->h_, m_yuvFrm_h->sz_);
	if( !hasNewFrm ){
		this->goToSleep();
		//dumpLog("DetThreadDeepNet():%s, go to sleep", m_threadName.c_str());
		return;
	}

	//do change detection and prepare data in <m_detFrm_h>
	bool suc = doChgDet();

	//wrt results into output queue
	m_camDc->m_frmInfoQ->wrtDetFrmByDetThread( m_detFrm_h.get() );
	if( m_dspPtr ){
		m_dspPtr->wakeupToWork();
	}
	else{
		//for unit test w/o <m_dspPtr>: we read data from que, otherwise the que will be overflowed
		DetFrm_h tmp;
		bool hasDetFrm = m_camDc->m_frmInfoQ->readDetFrmByDspThread( &tmp );
		if( hasDetFrm  && tmp.m_fn%m_frmFreqToLog==0){
			tmp.dump(".", "detFrm");
		}
	}

	if (m_yuvFrm_h->fn_ % m_frmFreqToLog == 0) {
		uint32_t dt = timeIntervalMillisec(start);
		dumpLog( "DetThreadDeepNet::procNextTask(): %s, fn=%llu, dt=%d", m_threadName.c_str(), m_yuvFrm_h->fn_, dt);
	}

}

bool DetThreadDeepNet::doChgDet()
{

	//preapre m_detFrm_h->m_rgbImg:  yuv-> resizedYuv -> rgb
	prepareDetImg();

	m_detFrm_h->m_vRois.clear();

	if (m_skipedFrmCount < m_detFrmsToSkip) {
			m_skipedFrmCount++;
	}
	else{
		m_skipedFrmCount = 0;
		//transfer to float4 img for shared cpu and gpu memory
		if (!DetThreadDeepNet::rgb2float4(m_detImgCpu, m_detImgW, m_detImgH, m_detFrm_h->m_rgbImg.I_)) {
			return false;
		}

		//do detection
		detectNet::Detection* detections = NULL;
		const int numDetections = m_deepNet->Detect(m_detImgGpu, m_detImgW, m_detImgH, &detections, m_overlayFlags);

		// print out the detection results
#if 0
		for (int n = 0; n < numDetections; n++) {
			printf("detected obj %u  class #%u (%s)  confidence=%f\n", detections[n].Instance, detections[n].ClassID, net->GetClassDesc(detections[n].ClassID), detections[n].Confidence);
			printf("bounding box %u  (%f, %f)  (%f, %f)  w=%f  h=%f\n", detections[n].Instance, detections[n].Left, detections[n].Top, detections[n].Right, detections[n].Bottom, detections[n].Width(), detections[n].Height());
		}
		// print out timing info
		m_deepNet->PrintProfilerTimes();
#endif

		//prepare results 
		for (int i = 0; i < numDetections; ++i) {
			const detectNet::Detection& d = detections[i];
			Roi tmp(d.Left, d.Top, d.Right - d.Left + 1, d.Bottom - d.Top + 1, d.ClassID, d.Confidence, std::string(m_deepNet->GetClassDesc(d.ClassID)));
			m_detFrm_h->m_vRois.push_back(tmp);
		}
	}
	prepareOutputImg();

	return true;
}

bool DetThreadDeepNet::procInit()
{
	//init this camera
	m_camDc = m_dcUI->m_dcMap[m_camId];
	CfgCam camCfg = m_cfg->getCam( m_camId );
	const int &w0 = camCfg.imgSz_.w;
	const int &h0 = camCfg.imgSz_.h;
	const int &L  = camCfg.detPyrLev_;


	//init currrent camera capture params
	m_detPyrL = L;
	m_detMethod = (DetectionMethod) camCfg.detMethodId_;
	m_detNetworkName = camCfg.detNetworkName_;
	m_detFrmsToSkip = camCfg.detFrmsToSkip_;  //if 1 do detection every other frm, skip 2 frames after one detetcion, ....
	m_detImgW = w0 >> L;
	m_detImgH = h0 >> L;

	m_yuvFrm_h.reset( new YuvFrm_h( w0, h0 ) );
	m_yuvFrmAtDetSz_h.reset( new YuvFrm_h( m_detImgW, m_detImgH ) );	
	m_detFrm_h.reset( new DetFrm_h( w0, h0, L ) );
	dumpLog( "DetThreadDeepNet::procInit():called! detL=%d, detW=%d,detH=%d", L, w0>>L, h0>>L );


	/*
	 * create detection network
	 */
	m_deepNet.reset( detectNet::Create(argc, argv) );
	if ( !m_deepNet.get() ) {
		dumpLog("DetThreadDeepNet::procInit(): failed to initialize detectNet.");
		return false;
	}
	// parse overlay flags
	m_overlayFlags = detectNet::OverlayFlagsFromStr("box,labels,conf");

	//allocate gpu memeory
	const int imgSzBytes = m_detImgW * m_detImgH * sizeof(float) * 4;   //RGBA interface to detectNet  m_deepNet->Detect()
	if ( !cudaAllocMapped((void**)&m_detImgCpu,  (void**)&m_imgGPU, imgSzBytes ) ) {
		dumpLog("DetThreadDeepNet::procInit(): failed to allocate %zu bytes", imgSzBytes);
		return false;
	}

	return true;
}


bool DetThreadDeepNet :: rgb2float4(float4* cpuRgba, int cpuImgW, int cpuImgH, const cv::Mat& bgrSrc)
{
	const int imgChs = bgrSrc.channels();
	if (3 != imgChs) {
		dumpLog("DetThreadDeepNet::rgb2float4(): image channels is not three!");
		return false;
	}

	if (bgrSrc.cols != cpuImgW || bgrSrc.rows != cpuImgH) {
		dumpLog("DetThreadDeepNet::rgb2float4(): image sizes do not match with eachother - Mat(w=%d,h=%d), cpuPtrSz(w=%d,h=%d)", 
			bgrSrc.cols, bgrSrc.rows, cpuImgW, cpuImgH));
		return false;
	}

	for (int y = 0; y < cpuImgH; y++){
		uint8_t* pSrc = bgrSrc.ptr(y);    //BGRBGR...BGR
		float4* pDes = cpuRgba + y * cpuImgW;
		for (int x = 0; x < cpuImgW; x++){
			uint8_t b = *pSrc++;
			uint8_t g = *pSrc++;
			uint8_t r = *pSrc++;
			*pDes++ = make_float4( (float)r /*R*/, (float)g /*G*/, (float)b/*B*/, 255.0f /*A*/);
		}
	}
	return true;
}

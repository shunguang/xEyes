/*
*------------------------------------------------------------------------
*RunXEyes.cpp
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
#include "RunXEyes.h"
#include "libCap/CapThreadSyn.h"
#include "libCap/CapSaveRtspH264.h"
#include "libDet/DetThreadBkgChg.h"
#include "libDet/DetThreadDeepNet.h"

using namespace std;
using namespace xeyes;

RunXEyes::RunXEyes(CfgPtr& cfg, QWidget* parent)
	: RunGui(cfg, parent)
	, m_vCapThreads()
	, m_vDetThreads()
	, m_vDspThreads()
	, m_vCamIds()
	, m_vDspFrm()
	, m_threadIdCnt(0)
{
	//init short cut to access <m_dcUI->m_dcMap[id]>
	m_cfg->getCamIds(m_vCamIds);
	for (auto &id : m_vCamIds) {
		cv::Size sz = m_cfg->getDspImgSz(id);
		DspFrm_hPtr dspFrm(new DspFrm_h(sz.width, sz.height) );
		m_vDspFrm.push_back(dspFrm);
	}

	//gui initialized at this point

	//create threads: order matters: do not chg 
	createDisplayThreads();
	createDetectionThreads();
	createCaptureThreads();


	int n = m_vCamIds.size();
	if (n >= 1) {
		QObject::connect(m_vDspThreads[0].get(), SIGNAL(sigReady2Disp()), this, SLOT(respns_dispImg0()), MY_QT_CONN);
	}
	if (n >= 2) {
		QObject::connect(m_vDspThreads[1].get(), SIGNAL(sigReady2Disp()), this, SLOT(respns_dispImg1()), MY_QT_CONN);
	}
	if (n >= 3) {
		QObject::connect(m_vDspThreads[2].get(), SIGNAL(sigReady2Disp()), this, SLOT(respns_dispImg2()), MY_QT_CONN);
	}
	if (n >= 4) {
		QObject::connect(m_vDspThreads[3].get(), SIGNAL(sigReady2Disp()), this, SLOT(respns_dispImg3()), MY_QT_CONN);
	}

	runAllThreads();
}


RunXEyes::~RunXEyes()
{
}

void RunXEyes::runAllThreads()
{
	startDisplayThreads();
	startDetectionThreads();
	startCaptureThreads();  //put capture as last in on purpose
}


void RunXEyes::createCaptureThreads()
{
	//create capture threads
	m_vCapThreads.clear();
	for (int i = 0; i < m_vCamIds.size(); ++i ) {
		int id = m_vCamIds[i];
		CfgCam currCfg = m_cfg->getCam(id);
		const string threadName = "CapThread4" + currCfg.cameraName_;
		CapThreadBasePtr cap;
		if( 0 ==currCfg.rtspUrl_.compare("none")  ){
			cap.reset(new CapThreadSyn(id, m_threadIdCnt, threadName));
		}
		else{
			cap.reset(new CapSaveRtspH264(id, m_threadIdCnt, threadName));
		}
		cap->setCfg(m_cfg);
		cap->setDcUI(m_dcUI);
		cap->setDetPtr(m_vDetThreads[i].get());

		m_vCapThreads.push_back(cap);
		m_threadIdCnt++;
	}
}

void RunXEyes::createDetectionThreads()
{
	m_vDetThreads.clear();
	for (int i = 0; i < m_vCamIds.size(); ++i) {
		int id = m_vCamIds[i];
		CfgCam currCfg = m_cfg->getCam(id);
		const string threadName = "DetThread4" + currCfg.cameraName_;

		DetThreadBasePtr det(0);
		DetectionMethod mtd = (DetectionMethod)currCfg.detMethodId_;
		if(  mtd == DET_MTD_CHG_DETECTION ){
			det.reset(new DetThreadBkgChg(id, m_threadIdCnt, threadName));
		}
		else{
			det.reset(new DetThreadDeepNet(id, m_threadIdCnt, threadName));
		}		
		det->setCfg(m_cfg);
		det->setDcUI(m_dcUI);
		det->setDspPtr(m_vDspThreads[i].get());

		m_vDetThreads.push_back(det);
		m_threadIdCnt++;
	}
}

void RunXEyes::createDisplayThreads()
{
	m_vDspThreads.clear();
	for (auto &id : m_vCamIds) {
		CfgCam currCfg = m_cfg->getCam(id);
		const string threadName = "DspThread4" + currCfg.cameraName_;
		DspThreadPtr dsp(new DspThread(id, m_threadIdCnt, threadName));

		dsp->setCfg(m_cfg);
		dsp->setDcUI(m_dcUI);
		m_vDspThreads.push_back(dsp);
		m_threadIdCnt++;
	}
}


void RunXEyes::startCaptureThreads()
{
	for (auto &x : m_vCapThreads) {
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if (x->isSleepMode()) {
				break;
			}
		}
		x->wakeupToWork();
		dumpLog("RunXEyes::startCaptureThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}

void RunXEyes::startDetectionThreads()
{
	for (auto &x : m_vDetThreads) {
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if (x->isSleepMode()) {
				break;
			}
		}
		//x->wakeupToWork();
		dumpLog("RunXEyes::startDetectionThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}

void RunXEyes::startDisplayThreads()
{
	for (auto &x : m_vDspThreads) {
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if (x->isSleepMode()) {
				break;
			}
		}
		//x->wakeupToWork();
		dumpLog("RunXEyes::startDisplayThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}


void RunXEyes::quitAllThreads()
{
	m_quitProgDlg->setProgress(20, "Quit capture threads ...");
	for (auto &x : m_vCapThreads) {
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}

	m_quitProgDlg->setProgress(50, "Quit detetcion threads ...");
	for (auto &x : m_vDetThreads) {
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}

	m_quitProgDlg->setProgress(80, "Quit didsplay threads ...");
	for (auto &x : m_vDspThreads) {
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}

	m_quitProgDlg->setProgress(90, "all Quited ...");
}


void RunXEyes::on_actionExit_triggered()
{
	createQuitDlg();
	quitAllThreads();
	closeQuitDlg();
	dumpLog("RunXEyes::on_actionExit_triggered(): exited!");
}

void RunXEyes::respns_dispImg0()
{
	dispImg(0);
}

void RunXEyes::respns_dispImg1()
{
	dispImg(1);
}

void RunXEyes::respns_dispImg2()
{
	dispImg(2);
}

void RunXEyes::respns_dispImg3()
{
	dispImg(3);
}

inline void RunXEyes::dispImg(const int camIdx)
{
	//shared data container of corresponding camera
	int camId = m_vCamIds[camIdx];
	Dc *pDC = m_dcUI->m_dcMap[camId].get();

	//read dspImg from shared DC
	DspFrm_h *p = m_vDspFrm[camIdx].get();
	bool suc = pDC->m_frmInfoQ->readDspFrmByGuiThread( p );
	if (suc) {
#if DSP_USE_QPIXMAP	
		m_ui->showImg(camIdx, p->m_img);
#else
		//todo		
#endif
	}
}

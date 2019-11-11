#include "TestXEyes.h"
using namespace std;
using namespace xeyes;

TestXEyes::TestXEyes(CfgPtr& cfg, QWidget* parent)
	: TestGui(cfg, parent)
	, m_vRunCaps()
	, m_threadIdCnt(0)
{
	//gui initialized at this point 
	createCaptureThreads();
	createDetectionThreads();
	createDisplayThreads();

	std::vector<int> vCamIds;
	m_cfg->getCamIds(vCamIds);
	for (auto &id : vCamIds) {
		m_vDcPtr.push_back( m_dcUI->m_dcMap[id].get() );
	}

	QObject::connect(m_vRunDsps[0].get(), SIGNAL(sigReady2Disp(const uint64_t)), this, SLOT(respns_dispImg0(const uint64_t)), MY_QT_CONN);
	QObject::connect(m_vRunDsps[1].get(), SIGNAL(sigReady2Disp(const uint64_t)), this, SLOT(respns_dispImg1(const uint64_t)), MY_QT_CONN);
	QObject::connect(m_vRunDsps[2].get(), SIGNAL(sigReady2Disp(const uint64_t)), this, SLOT(respns_dispImg2(const uint64_t)), MY_QT_CONN);
	QObject::connect(m_vRunDsps[3].get(), SIGNAL(sigReady2Disp(const uint64_t)), this, SLOT(respns_dispImg3(const uint64_t)), MY_QT_CONN);

	runAllThreads();
}


TestXEyes::~TestXEyes()
{
}

void TestXEyes::runAllThreads()
{
	startCaptureThreads();
	startDetectionThreads();
	startDisplayThreads();
}


void TestXEyes::createCaptureThreads()
{
	//get cam ids from cfg
	std::vector<int> vCamIds;
	m_cfg->getCamIds(vCamIds);

	//create capture threads
	for (auto &id : vCamIds) {
		CfgCam currCfg = m_cfg->getCam(id);
		const string threadName = "CapThread4" + currCfg.cameraName_;
		RunCapBasePtr capA(new RunCapCamA(id, m_threadIdCnt, threadName));

		capA->setCfg(m_cfg);
		capA->setDcUI(m_dcUI);
		m_vRunCaps.push_back(capA);
		m_threadIdCnt++;
	}
}

void TestXEyes::createDetectionThreads()
{
}

void TestXEyes::createDisplayThreads()
{
}


void TestXEyes::startCaptureThreads()
{
	return;
	for (auto &x : m_vRunCaps) {
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if (x->isSleepMode()) {
				break;
			}
		}
		x->wakeupToWork();
		dumpLog("TestCaps::startThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}

void TestXEyes::startDetectionThreads()
{
}

void TestXEyes::startDisplayThreads()
{
}


void TestXEyes::quitAllThreads()
{
	m_quitProgDlg->setProgress(20, "Quit capture threads ...");
	for (auto &x : m_vRunCaps) {
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}

	m_quitProgDlg->setProgress(40, "Quit detetcion threads ...");

	m_quitProgDlg->setProgress(80, "Quit didsplay threads ...");

}


void TestXEyes::on_actionExit_triggered()
{
	createQuitDlg();
	quitAllThreads();
	closeQuitDlg();
	dumpLog("TestXEyes::on_actionExit_triggered(): exited!");
}

void TestXEyes::respns_dispImg0(const uint64_t fn)
{
	m_vDcPtr[0]->m_frmInfoQ->readDsp( m_dspFrm_h.get() );
	m_ui->showImg(0, m_dspFrm_h->m_img);
}

void TestXEyes::respns_dispImg1(const uint64_t fn)
{
	m_vDcPtr[1]->m_frmInfoQ->readDsp(m_dspFrm_h.get());
	m_ui->showImg(1, m_dspFrm_h->m_img);
}

void TestXEyes::respns_dispImg2(const uint64_t fn)
{
	m_vDcPtr[2]->m_frmInfoQ->readDsp(m_dspFrm_h.get());
	m_ui->showImg(2, m_dspFrm_h->m_img);
}

void TestXEyes::respns_dispImg3(const uint64_t fn)
{
	m_vDcPtr[3]->m_frmInfoQ->readDsp(m_dspFrm_h.get());
	m_ui->showImg(3, m_dspFrm_h->m_img);
}
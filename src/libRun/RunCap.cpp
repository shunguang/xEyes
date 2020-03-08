#include "RunCap.h"

using namespace std;
using namespace xeyes;
using namespace boost::posix_time;
RunCap::RunCap(CfgPtr &cfg)
	: m_cfg(cfg)
	, m_dcUI(0)
	, m_vCapThreads()
	, m_mainLoopExited(false)
	, m_exitMainLoop(false)
{
	dumpLog( "RunCap::RunCap(): start!" );
	CfgLog logCfg = m_cfg->getLog();
	myCreateDir( logCfg.logFolder );

	cout << "RunCap::RunCap(): cfg=" << cfg->toString() << endl;
	m_dcUI.reset( new DcUI(cfg) );

	//get cam ids from cfg
	std::vector<int> vCamIds; 
	m_cfg->getCamIds( vCamIds );

	//create capture threads
	int threadId=0;
	for( auto &id : vCamIds ){
		CfgCam currCfg = m_cfg->getCam( id );
		const string threadName = "Thread4" + currCfg.cameraName_;
		CapThreadBasePtr cap(NULL);
		if( 0 ==currCfg.rtspUrl_.compare("none") ){
			cap.reset(new CapThreadSyn(id, threadId, threadName));
		}
		else{
			cap.reset(new CapSaveRtspH264(id, threadId, threadName));
		}
		cap->setCfg( m_cfg );
		cap->setDcUI( m_dcUI );
		m_vCapThreads.push_back( cap );
		threadId++;
}

	dumpLog( "RunCap::RunCap(): done!" );
}

RunCap::~RunCap()
{
}

void RunCap::runAllThreads()
{
	m_exitMainLoop = false;
	m_mainLoopExited = false;

	dumpLog( "RunCap::runAllThreads(): start!" );

	for( auto &x : m_vCapThreads){
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if ( x->isSleepMode()) {
				break;
			}
		}
		x->wakeupToWork();
		dumpLog("RunCap::startThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}

	//waiting for input from keyboard
	char c;
	cin >> c;
	while(c != 'q'){
		cin >> c;
	}

	quitAllThreads();
	m_mainLoopExited = true;
	dumpLog("RunCap::startThreads(): exited!");
}


void RunCap::quitAllThreads()
{
	for( auto &x : m_vCapThreads){
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}
	m_exitMainLoop = true;
}






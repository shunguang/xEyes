#include "TestCaps.h"

using namespace std;
using namespace xeyes;
using namespace boost::posix_time;
TestCaps::TestCaps(CfgPtr &cfg)
	: m_cfg(cfg)
	, m_dcUI(0)
	, m_vRunCaps()
	, m_mainLoopExited(false)
	, m_exitMainLoop(false)
{
	CfgLog logCfg = m_cfg->getLog();
	myCreateDir( logCfg.logFolder );


	m_dcUI.reset( new DcUI(cfg) );

	//get cam ids from cfg
	std::vector<int> vCamIds; 
	m_cfg->getCamIds( vCamIds );

	//create capture threads
	int threadId=0;
	for( auto &id : vCamIds ){
		CfgCam currCfg = m_cfg->getCam( id );
		const string threadName = "Thread4" + currCfg.cameraName_;
		RunCapBasePtr capA( new RunCapCamA( id, threadId++, threadName) );

		capA->setCfg( m_cfg );
		capA->setDcUI( m_dcUI );
		m_vRunCaps.push_back( capA );
	}
}

TestCaps::~TestCaps()
{
}

void TestCaps::runAllThreads()
{
	m_exitMainLoop = false;
	m_mainLoopExited = false;

	for( auto &x : m_vRunCaps){
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if ( x->isSleepMode()) {
				break;
			}
		}
		x->wakeupToWork();
		dumpLog("TestCaps::startThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}

	//waiting for input from keyboard
	char c;
	cin >> c;
	while(c != 'q'){
		cin >> c;
	}

	quitAllThreads();
	m_mainLoopExited = true;
	dumpLog("TestCaps::startThreads(): exited!");
}


void TestCaps::quitAllThreads()
{
	for( auto &x : m_vRunCaps){
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}
	m_exitMainLoop = true;
}






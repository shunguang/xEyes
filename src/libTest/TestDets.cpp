#include "TestDets.h"

using namespace std;
using namespace xeyes;
using namespace boost::posix_time;
TestDets::TestDets(CfgPtr &cfg)
	: m_cfg(cfg)
	, m_dcUI(0)
	, m_vRunCaps()
	, m_mainLoopExited(false)
	, m_exitMainLoop(false)
    , m_threadId( 0 )
    , m_vCamIds()
{
	CfgLog logCfg = m_cfg->getLog();
	myCreateDir( logCfg.logFolder );


	m_dcUI.reset( new DcUI(cfg) );

	//get cam ids from cfg
	m_cfg->getCamIds( m_vCamIds );
	
    //create capture threads
    createCapThreads();
    createDetThreads();

    //set detection method thread pointer to corresponding capther threads
    //then we can wake up the detection threads when a new frame is captured.
    for( int i=0; i<m_vCamIds.size(); ++i ){
        m_vRunCaps[i]->setDetPtr( m_vRunDets[i].get() );
    }
}

TestDets::~TestDets()
{
}

void TestDets::startAllThreads()
{
	m_exitMainLoop = false;
	m_mainLoopExited = false;
    
    startCapThreads();
    startDetThreads();

	//waiting for input from keyboard
	char c;
	cin >> c;
	while(c != 'q'){
		cin >> c;
	}

	quitAllThreads();
	m_mainLoopExited = true;
	dumpLog("TestDets::startThreads(): exited!");
}


void TestDets::quitAllThreads()
{
	for( auto &x : m_vRunCaps){
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}
	m_exitMainLoop = true;
}


void TestDets::createCapThreads()
{
	for( auto &id : m_vCamIds ){
		CfgCam currCfg = m_cfg->getCam( id );
		const string threadName = "Cap thread for" + currCfg.cameraName_;
		RunCapBasePtr capA( new RunCapCamA( id, m_threadId, threadName) );

		capA->setCfg( m_cfg );
		capA->setDcUI( m_dcUI );
		m_vRunCaps.push_back( capA );

        m_threadId++;
	}
}

void TestDets::createDetThreads()
{
	for( auto &id : m_vCamIds ){
		CfgCam currCfg = m_cfg->getCam( id );
		const string threadName = "Det thread for" + currCfg.cameraName_;
		RunDetBasePtr det( new RunDetBkgChg( id, m_threadId, threadName) );

		det->setCfg( m_cfg );
		det->setDcUI( m_dcUI );
		m_vRunDets.push_back( det );

        m_threadId++;
	}
}

void TestDets::startCapThreads()
{
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
		dumpLog("TestDets::startCapThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}

void TestDets::startDetThreads()
{
	for( auto &x : m_vRunDets){
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if ( x->isSleepMode()) {
				break;
			}
		}
		dumpLog("TestDets::startDetThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}




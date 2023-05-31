/*
*------------------------------------------------------------------------
*RunCap.cpp
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

void RunCap::startAllThreads()
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






/*
*------------------------------------------------------------------------
*RunDet.cpp
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
#include "RunDet.h"

using namespace std;
using namespace xeyes;
using namespace boost::posix_time;
RunDet::RunDet(CfgPtr &cfg)
	: m_cfg(cfg)
	, m_dcUI(0)
	, m_vCapThreads()
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
        m_vCapThreads[i]->setDetPtr( m_vDetThreads[i].get() );
    }
}

RunDet::~RunDet()
{
}

void RunDet::startAllThreads()
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
	dumpLog("RunDet::startThreads(): exited!");
}


void RunDet::quitAllThreads()
{
	for( auto &x : m_vCapThreads){
		x->forceQuit();
		dumpLog("Thread: id=%d, name=%s quited!", x->m_threadId, x->m_threadName.c_str());
	}
	m_exitMainLoop = true;
}


void RunDet::createCapThreads()
{
	for( auto &id : m_vCamIds ){
		CfgCam currCfg = m_cfg->getCam( id );
		const string threadName = "Cap thread for" + currCfg.cameraName_;

		CapThreadBasePtr cap(NULL);
		if( 0 ==currCfg.rtspUrl_.compare("none") ){
			cap.reset(new CapThreadSyn(id, m_threadId, threadName));
		}
		else{
			cap.reset(new CapSaveRtspH264(id, m_threadId, threadName));
		}
		cap->setCfg( m_cfg );
		cap->setDcUI( m_dcUI );
		m_vCapThreads.push_back( cap );
        m_threadId++;
	}
}

void RunDet::createDetThreads()
{
	for( auto &id : m_vCamIds ){
		CfgCam currCfg = m_cfg->getCam( id );
		const string threadName = "Det thread for" + currCfg.cameraName_;
		DetThreadBasePtr det( new DetThreadBkgChg( id, m_threadId, threadName) );

		det->setCfg( m_cfg );
		det->setDcUI( m_dcUI );
		m_vDetThreads.push_back( det );

        m_threadId++;
	}
}

void RunDet::startCapThreads()
{
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
		dumpLog("RunDet::startCapThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}

void RunDet::startDetThreads()
{
	for( auto &x : m_vDetThreads){
		x->start();
		//make sure every capture thread was initialized
		while (1) {
			THREAD_SLEEP(10);
			if ( x->isSleepMode()) {
				break;
			}
		}
		dumpLog("RunDet::startDetThreads(): ThreadId=%d, name=%s started.", x->m_threadId, x->m_threadName.c_str());
	}
}




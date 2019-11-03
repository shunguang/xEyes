#include "ThreadX.h"
using namespace std;
using namespace xeyes;

ThreadX::ThreadX(const ThreadTaskId tid, const int threadId, const std::string &name)
	: m_cfg(0)
	, m_dcUI(0)
	, m_taskId(tid)
	, m_threadId( threadId )
	, m_threadName(name)
	, m_frmFreqToLog(100)
	, m_isExitedLoop(false)
	, m_isSleepMode(true)
	, m_rcvdExitLoopCmd(false)
	, m_threadX(0)
	, m_mutex4Working()
	, m_condition4Working()
{
	//cout << "ThreadX::ThreadX(): called" << endl;
}


ThreadX::~ThreadX()
{
	m_threadX->join();
}

void ThreadX::setCfg(CfgPtr &cfg)
{
	//cout << "ThreadX::setCfg(): AAA" << endl;
	m_cfg = cfg;
	initFrmFreq2Log( m_taskId );
	//cout << "ThreadX::setCfg(): called" << endl;
}


void ThreadX::setDcUI(DcUIPtr &dcUI )
{
	m_dcUI = dcUI;
	//cout << "ThreadX::setDC(): called" << endl;
}

void ThreadX::start()
{
	procInit();
	m_threadX.reset(new boost::thread(boost::bind(&ThreadX::runLoop, this)));
}

void ThreadX::runLoop()
{
	bool quitLoop = false;
	while (!quitLoop) {
		{
			boost::mutex::scoped_lock lock(m_mutex4Working);
			while (m_isSleepMode) {
				//dumpLog("ThreadX::run(): in sleep mode--%s", m_threadName.c_str());
				m_condition4Working.wait(lock);
			}
			//dumpLog("ThreadX::run(): waked up--%s", m_threadName.c_str());
		}

		quitLoop = isRcvdExitLoopCmd();
		if (quitLoop) {
			break;
		}

		procNextTask();

		quitLoop = isRcvdExitLoopCmd();
	}
	dumpLog("ThreadX::run(): exit loop--%s", m_threadName.c_str());

	{
		boost::mutex::scoped_lock lock(m_mutex4Working);
		m_isExitedLoop = true;
	}
}

void ThreadX::forceQuit()
{
	{
		boost::mutex::scoped_lock lock(m_mutex4Working);
		m_rcvdExitLoopCmd = true;
	}
	if ( isSleepMode() ) {
		wakeupToWork();
	}

	int cnt = 0;
	while (!isExitedLoop()) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(10));
		++cnt;
		if (cnt > 500) {
			dumpLog(" stuck at: forceThreadQuit() at thread %s", m_threadName.c_str());
			cnt = 0;
		}
	}

}

bool ThreadX::isExitedLoop()
{
	bool f;
	{
		boost::mutex::scoped_lock lock(m_mutex4Working);
		f = m_isExitedLoop;
	}
	return f;
}

bool ThreadX::isRcvdExitLoopCmd()
{
	bool f;
	{
		boost::mutex::scoped_lock lock(m_mutex4Working);
		f = m_rcvdExitLoopCmd;
	}
	return f;

	}

bool ThreadX::isSleepMode()
{
	bool f;
	{
		boost::mutex::scoped_lock lock(m_mutex4Working);
		f = m_isSleepMode;
	}
	return f;
}


void ThreadX::wakeupToWork()
{
	boost::mutex::scoped_lock lock(m_mutex4Working);
	m_isSleepMode = false;
	m_condition4Working.notify_one();
}

void ThreadX::goToSleep()
{
	boost::mutex::scoped_lock lock(m_mutex4Working);
	m_isSleepMode = true;
}

void ThreadX::initFrmFreq2Log(const ThreadTaskId tid )
{
	CfgLog cfg = m_cfg->getLog();
	if (tid >= 0 && tid < THD_TASK_COUNT) {
		m_frmFreqToLog = cfg.vFrmFreqToLog[tid];
	}
	else{
		m_frmFreqToLog = 1000;
	}
}

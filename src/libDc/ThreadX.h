#ifndef __THREAD_X_H__
#define __THREAD_X_H__

#include "DsDefs.h"
#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "DcUI.h"
namespace xeyes {
	class DS_EXPORT ThreadX {
	public:
		ThreadX(const ThreadTaskId tid, const int threadId=0, const std::string &name="unkn" );
		virtual ~ThreadX();

		void start();            //start thread
		void forceQuit();        //force thread quit from its inifinity loop

		void wakeupToWork();     //wake up thread to  procNextTask()
		void goToSleep();        //ask thread go to sleep
		bool isExitedLoop();
		bool isSleepMode();

		void setCfg( CfgPtr  &cfg );
		void setDcUI( DcUIPtr &dcUI );

	protected:
		void setExitedLoopFlag(const bool f);
		void setRcvdExitLoopCmdFlag(const bool f);
		bool isRcvdExitLoopCmd();

		virtual void procNextTask() = 0;
		virtual bool procInit() = 0;

		void runLoop();
		void initFrmFreq2Log( const ThreadTaskId id );

	public:
		ThreadTaskId	m_taskId;
		int 			m_threadId;		//for debugging purpose
		std::string		m_threadName;	//for debugging purpose

	protected:
		CfgPtr			m_cfg;			//cfg ptr
		DcUIPtr			m_dcUI;			//shared data pointer
		int             m_frmFreqToLog;

		bool    		m_isExitedLoop;
		bool			m_isSleepMode;
		bool    		m_rcvdExitLoopCmd;

		std::shared_ptr<boost::thread>	 m_threadX;
		boost::mutex					 m_mutex4Working;
		boost::condition_variable		 m_condition4Working;
		boost::mutex					 m_mutexLocal;
	};
	typedef std::shared_ptr<ThreadX>		ThreadXPtr;
}
#endif

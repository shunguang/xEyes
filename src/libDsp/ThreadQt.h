/*
*-------------------------------------------------------------------
* ThreadQt.h
* Copyright(c) 2019, The RgbVideoEditor Author (Shunguang Wu).
* All Rights Reserved.
* You may not use this file except in compliance with the License:
*      GNU GENERAL PUBLIC LICENSE (Version 3), 29 June 2007
*
* You may obtain a copy of the License at
*
*     https://www.gnu.org/licenses/gpl-3.0.en.html
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*-------------------------------------------------------------------
*/
#ifndef __THREAD_QT_H__
#define __THREAD_QT_H__

#include <QtCore/qthread.h>
#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "libDs/DcUI.h"
#include "DspDefs.h"
namespace xeyes {
	class DSP_EXPORT ThreadQt : public QThread {
		Q_OBJECT
	public:
		ThreadQt(const ThreadTaskId tid, const int threadId=0, const std::string &name="unkn" );
		virtual ~ThreadQt();

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

		void run() Q_DECL_OVERRIDE;            //must be named run() at here
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

		boost::mutex					 m_mutex4Working;
		boost::condition_variable		 m_condition4Working;
		boost::mutex					 m_mutexLocal;
	};
	typedef std::shared_ptr<ThreadQt>		ThreadQtPtr;
}
#endif

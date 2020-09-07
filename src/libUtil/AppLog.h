/*
*------------------------------------------------------------------------
*AppLog.h
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
#ifndef __APP_LOG_H__
#define __APP_LOG_H__

#include <stdarg.h>
#include <fstream>
#include <time.h>

#include "DataTypes.h"
#include "UtilDefs.h"
#include "FileUtil.h"
#define LOG_USING_LOCAL_TIME  1
#define LOG_USE_BOOST_MUTEX   1		//0 - use linux POSIX mutex
#define	LOG_MAX_MSG_LEN		1024

namespace xeyes {
	//--------- log UI funcs------------
	void  UTIL_EXPORT startLogThread(const std::string &logFilename, const bool showInConsole = true, const bool dumpLog = true);
	void  UTIL_EXPORT endLogThread();

	void  UTIL_EXPORT dumpLog(const char * x, ...);
	void  UTIL_EXPORT dumpLog(const  std::string &x, ...);

	void  UTIL_EXPORT myExit(const int flag);
	void  UTIL_EXPORT myExit(const char * x, ...);
	void  UTIL_EXPORT myExit(const  std::string &x, ...);
	void  UTIL_EXPORT myAssert(const bool flag, const std::string &msg);
	void  UTIL_EXPORT myAssert(const bool flag, const char *file, const int lineNum);

	//----------- def of AppLog -------------------
	class UTIL_EXPORT AppLog {
	private:
		AppLog();

	public:
		AppLog(const AppLog &x) = delete;            //donot implement
		void operator=(const AppLog &x) = delete;    //donot implement
		
		static void createLogInstance(const std::string &logFilePath) {
			if( m_logPtr == NULL ){
				m_logPtr = new AppLog();
				AppLog::m_logFilename = logFilePath;
			}
		}

		void logMsg(const std::string &msg);
		void logMsg(const char* msg);
		void startLog();
		void endLog();

	private:
		void doDumpLoop();
		std::string getTime();
		bool getNextMsg(std::string &curMsg) {
			bool f = false;
			{
				boost::mutex::scoped_lock lock(m_logOtherMutex);
				if (!m_msgQ.empty()) {
					curMsg = m_msgQ.front(); //hard copy
					m_msgQ.pop_front();
					f = true;
				}
			}
			return f;
		}

		inline void wakeUpToWork() {
			boost::mutex::scoped_lock lock(m_logSleepMutex);
			m_goSleep = false;
			m_logCondition.notify_one();
		}

		inline void setForceExit( bool f) {
			boost::mutex::scoped_lock lock(m_logOtherMutex);
			m_forceExit = f;
		}

		inline bool isForceExit() {
			bool f;
			{
				boost::mutex::scoped_lock lock(m_logOtherMutex);
				f = m_forceExit;
			}
			return f;
		}

		inline void setLoopExited(bool f) {
			boost::mutex::scoped_lock lock(m_logOtherMutex);
			m_loopExited = f;
		}

		bool isLoopExited() {
			bool f;
			{
				boost::mutex::scoped_lock lock(m_logOtherMutex);
				f = m_loopExited;
			}
			return f;
		}

	private:
		// --- m_goSleep guided by m_logSleepMutex ---
		boost::condition_variable	m_logCondition;
		boost::mutex				m_logSleepMutex;
		bool m_goSleep;
		
		// --- other variables guided by m_logOtherMutex ---
		boost::mutex				m_logOtherMutex;
		bool m_forceExit;
		bool m_loopExited;
		std::deque<std::string> m_msgQ;
		
		//--------------
		std::time_t 					 m_begTime;
		std::shared_ptr<boost::thread>	 m_logThread;

	public:
		//todo: This isn't thread safe. Better to make <m_logPtr> a local static of createLogInstance()
		//and initialize it immediately without a test
		static AppLog* 	   m_logPtr;
		static std::string m_logFilename;
		static bool        m_logShowMsgInConsole;
		static bool        m_logIsDump;
	};
}
#endif


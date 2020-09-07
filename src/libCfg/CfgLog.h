/*
*------------------------------------------------------------------------
*CfgLog.h
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
#ifndef __CFG_LOG_H__
#define __CFG_LOG_H__

#include "CfgBase.h"
namespace xeyes {
	class  CFG_EXPORT CfgLog : public CfgBase {
	public:
		CfgLog();
		CfgLog( const CfgLog &x );
		CfgLog& operator = (const CfgLog &x);
		virtual ~CfgLog() {};

		virtual boost::property_tree::ptree toPropertyTree();
		virtual void fromPropertyTree(const boost::property_tree::ptree &pt);
		virtual std::string toString();

	public:
		std::string appRootFolder;
		std::string logFolder;
		bool        isDumpLog;
		bool        ishowLogInConsole;

		//for server vFrmFreqToLog[0] - dump capture thread log every this frame
		//           vFrmFreqToLog[1] - dump runcuda thread log every this frame
		//            ...  
		int			vFrmFreqToLog[THD_TASK_COUNT];
	};
	typedef std::shared_ptr<CfgLog>		CfgLogPtr;
}

#endif

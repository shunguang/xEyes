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

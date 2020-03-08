//used to test libCap
#ifndef __RUN_CAP_H__
#define __RUN_CAP_H__

#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "libDc/DcUI.h"
#include "libCap/CapSaveRtspH264.h"
#include "libCap/CapThreadSyn.h"
namespace xeyes {
	class RunCap {
	public:
		RunCap(CfgPtr &cfg);
		~RunCap();
		void runAllThreads();

	protected:	
		void quitAllThreads();

	private:
		CfgPtr	m_cfg;	//accessed by multi-threads
		DcUIPtr	m_dcUI;	//shared data containers among threads

		std::vector<CapThreadBasePtr>	m_vCapThreads;

		bool	m_exitMainLoop;
		bool	m_mainLoopExited;
	};
}
#endif

//used to test libCap
#ifndef __TEST_CAPS_H__
#define __TEST_CAPS_H__

#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "libDs/DcUI.h"
#include "libCap/CapThreadCamTemp.h"
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

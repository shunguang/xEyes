//used to test libCap and libDet
#ifndef __RUN_DET_H__
#define __RUN_DET_H__

#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "libDs/DcUI.h"
#include "libCap/CapThreadCamTemp.h"
#include "libDet/DetThreadBkgChg.h"
namespace xeyes {
	class RunDet {
	public:
		RunDet(CfgPtr &cfg);
		~RunDet();
		void startAllThreads();

    private:
		void quitAllThreads();
		void createCapThreads();
        void createDetThreads();
        void startCapThreads();
        void startDetThreads();
        
	private:
		CfgPtr	m_cfg;	//accessed by multi-threads
		DcUIPtr	m_dcUI;	//shared data containers among threads

		std::vector<CapThreadBasePtr>	m_vCapThreads;   //capture threads
		std::vector<DetThreadBasePtr>	m_vDetThreads;   //detection threads

		bool	m_exitMainLoop;
		bool	m_mainLoopExited;
        int 	m_threadId;
    	std::vector<int> m_vCamIds; 
	};
}
#endif

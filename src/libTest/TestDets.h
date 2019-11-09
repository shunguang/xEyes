//used to test libCap and libDet
#ifndef __TEST_DETS_H__
#define __TEST_DETS_H__

#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "libDs/DcUI.h"
#include "libCap/RunCapCamA.h"
#include "libDet/RunDetBkgChg.h"
namespace xeyes {
	class TestDets {
	public:
		TestDets(CfgPtr &cfg);
		~TestDets();
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

		std::vector<RunCapBasePtr>	m_vRunCaps;   //capture threads
		std::vector<RunDetBasePtr>	m_vRunDets;   //detection threads

		bool	m_exitMainLoop;
		bool	m_mainLoopExited;
        int 	m_threadId;
    	std::vector<int> m_vCamIds; 
	};
}
#endif

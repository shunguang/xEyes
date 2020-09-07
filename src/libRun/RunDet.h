/*
*------------------------------------------------------------------------
*RunDet.h
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
//used to test libCap and libDet
#ifndef __RUN_DET_H__
#define __RUN_DET_H__

#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "libDc/DcUI.h"
#include "libCap/CapThreadSyn.h"
#include "libCap/CapSaveRtspH264.h"
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

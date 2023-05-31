/*
*------------------------------------------------------------------------
*RunCap.h
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
		void startAllThreads();

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

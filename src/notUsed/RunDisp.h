/*
*------------------------------------------------------------------------
*RunDisp.h
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
//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_DISP_H__
#define __RUN_DISP_H__
#include "libUtil/util.h"
#include "libDc/ThreadX.h"
#include "libDc/RgbFrm_h.h"
#include "GuiDefs.h"
namespace xeyes {
	class GUI_EXPORT RunDisp : public ThreadX
	{
	public:
		RunDisp( const int threadId=0, const std::string &threadName="unkn");
		RunDisp( const RunDisp &x) = delete;
		virtual ~RunDisp();

	protected:
		//set <isEmitSignal> as false when doing single thread none GUI test
		virtual void procNextTask();
		virtual bool procInit();
		void 		 dumpDispFrm( const std::string &folderPath );

	protected:
		uint64			m_frmNum;
		CfgLocalView	m_lvCfg;
		std::vector<int> m_vCamId;

		//class tmp variable
		RgbFrm_hPtr		m_DspFrm_h;    //a frm come from multi-camera
		RgbFrm_hPtr		m_camFrm_h;     //1 frm from once camera
	};
	typedef std::shared_ptr<RunDisp> RunDispPtr;
}
#endif

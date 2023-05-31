/*
*------------------------------------------------------------------------
*RunDisp-tmp.h
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
#ifndef __RUN_DET_BKG_CHG_H__
#define __RUN_DET_BKG_CHG_H__
#include "RunDetBase.h"
namespace xeyes {
	class DET_EXPORT RunDetBkgChg : public RunDetBase
	{
	public:
		RunDetBkgChg(const int camId=0, const int threadId=0, const std::string &threadName="det_thread");
		~RunDetBkgChg();

	protected:
		virtual void 	procNextTask();
		virtual bool 	procInit();
		bool 			doChgDet();
	};
	typedef std::shared_ptr<RunDetBkgChg> RunDetBkgChgPtr;
}
#endif // CLIENTGUI_H

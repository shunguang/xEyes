/*
*------------------------------------------------------------------------
*CapThreadBase.h
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
//This is teh base class for capture threads
#ifndef __CAP_THREAD_BASE_H__
#define __CAP_THREAD_BASE_H__
#include "libUtil/util.h"
#include "libDc/ThreadX.h"
#include "libDc/YuvFrm_h.h"
#include "CapDefs.h"
namespace xeyes {
	class DetThreadBase;
	class CAP_EXPORT CapThreadBase : public ThreadX
	{
	public:
		CapThreadBase( const int camId=0, const int threadId=0, const std::string &threadName="unkn");
		CapThreadBase( const CapThreadBase &x) = delete;
		virtual ~CapThreadBase();
		void setDetPtr( DetThreadBase *p );

	protected:
		//set <isEmitSignal> as false when doing single thread none GUI test
		virtual void procNextTask()=0;
		virtual bool procInit()=0;
		void 		 dumpYuvFrm( const std::string &folderPath );

	protected:
		int 			m_camId;
		uint64			m_frmNum;
		int				m_frmInterval_ms;

		YuvFrm_hPtr		m_yuvFrm_h;   //output/captured frm from this camera
		DcPtr 			m_camDc;      //shared data container of this camera
		CfgCam			m_camCfg;     //hd copy this camera cfg
		DetThreadBase*	m_detPtr;
	};
	typedef std::shared_ptr<CapThreadBase> CapThreadBasePtr;
}
#endif 

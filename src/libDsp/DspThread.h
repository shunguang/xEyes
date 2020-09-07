/*
*------------------------------------------------------------------------
*DspThread.h
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
#ifndef __DSP_THREAD_H__
#define __DSP_THREAD_H__
#include "libUtil/util.h"
#include "libUtil/CvQtUtil.h"
#include "libDc/ThreadX.h"
#include "libDc/DetFrm_h.h"
#include "libDc/DspFrm_h.h"
#include "libDc/YuvFrm_h.h"

#include "ThreadQt.h"
#include "DspDefs.h"

#include <QtCore/QObject>
namespace xeyes {
	class DSP_EXPORT DspThread : public ThreadQt
	{
		Q_OBJECT
	public:
		DspThread( const int camId=0, const int threadId=0, const std::string &threadName="unkn");
		DspThread( const DspThread &x) = delete;
		virtual ~DspThread();

		void dumpFrm(const std::string &folderPath);
		cv::Size  getDspImgSz() {
			return m_dspSz;
		}

	signals:
		void sigReady2Disp();

	protected:
		//set <isEmitSignal> as false when doing single thread none GUI test
		virtual void procNextTask();
		virtual bool procInit();
		bool		 prepareDspImg();

	protected:
		int 			m_camId;
		int             m_detPyrL;
		int             m_dspPyrL;
		cv::Size		m_dspSz;
		cv::Size		m_detSz;

		DetFrm_hPtr		m_detFrm_h;   //input: detetcion results

		DspFrm_hPtr		m_dspFrm_h;   //ouput: binded original frm with detetcion results
		DcPtr 			m_camDc;      //shared data container of this camera

		//temporal varaibles
		cv::Mat			m_dspImg;
	};
	typedef std::shared_ptr<DspThread> DspThreadPtr;
}
#endif 

//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __DET_THREAD_BASE_H__
#define __DET_THREAD_BASE_H__
#include "libUtil/util.h"
#include "libDs/ThreadX.h"
#include "libDs/YuvFrm_h.h"
#include "libDs/RgbFrm_h.h"
#include "DetDefs.h"
namespace xeyes {
	class DspThread;
	class DET_EXPORT DetThreadBase : public ThreadX
	{
	public:
		DetThreadBase( const int camId=0, const int threadId=0, const std::string &threadName="unkn");
		DetThreadBase( const DetThreadBase &x) = delete;
		virtual ~DetThreadBase();
		void setDspPtr(DspThread *p);
	protected:
		//set <isEmitSignal> as false when doing single thread none GUI test
		virtual void procNextTask()=0;
		virtual bool procInit()=0;
		void 		 dumpFrm( const std::string &folderPath );

	protected:
		int 			m_camId;
		uint64			m_frmNum;
		int             m_detPyrL;
		  
		YuvFrm_hPtr		m_yuvFrm_h;   //input/captured frm to process
		DetFrm_hPtr		m_detFrm_h;   //output detetcion frm after processing

		DcPtr 			m_camDc;      //shared data container of this camera
		DspThread*			m_dspPtr;
	};
	typedef std::shared_ptr<DetThreadBase> DetThreadBasePtr;
}
#endif 

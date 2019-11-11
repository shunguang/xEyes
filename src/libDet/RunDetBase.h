//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_DET_BASE_H__
#define __RUN_DET_BASE_H__
#include "libUtil/util.h"
#include "libDs/ThreadX.h"
#include "libDs/YuvFrm_h.h"
#include "libDs/RgbFrm_h.h"
#include "DetDefs.h"
namespace xeyes {
	class RunDsp;
	class DET_EXPORT RunDetBase : public ThreadX
	{
	public:
		RunDetBase( const int camId=0, const int threadId=0, const std::string &threadName="unkn");
		RunDetBase( const RunDetBase &x) = delete;
		virtual ~RunDetBase();
		void setDspPtr(RunDsp *p);
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
		RunDsp*			m_dspPtr;
	};
	typedef std::shared_ptr<RunDetBase> RunDetBasePtr;
}
#endif 

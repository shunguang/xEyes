//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __DET_THREAD_BASE_H__
#define __DET_THREAD_BASE_H__
#include "libUtil/util.h"
#include "libDc/ThreadX.h"
#include "libDc/YuvFrm_h.h"
#include "libDc/RgbFrm_h.h"
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
		void 		 prepareDetImg();
		void 		 prepareOutputImg();
		void 		 dumpFrm( const std::string &folderPath );

	protected:
		int 			m_camId;
		uint64			m_frmNum;

		int             m_detPyrL;
		DetectionMethod	m_detMethod;
		std::string		m_detNetworkName;
		int				m_detFrmsToSkip;  //if 1 do detection every other frm, skip 2 frames after one detetcion, ....


		YuvFrm_hPtr		m_yuvFrm_h;   		 //input/captured frm to process
		YuvFrm_hPtr		m_yuvFrmAtDetSz_h;   //resized yuv if need
		DetFrm_hPtr		m_detFrm_h;   		 //output detetcion frm after processing

		DcPtr 			m_camDc;			//shared data container of this camera
		DspThread*		m_dspPtr;	
		int				m_skipedFrmCount;	//frms skiped for detetction
	};
	typedef std::shared_ptr<DetThreadBase> DetThreadBasePtr;
}
#endif 

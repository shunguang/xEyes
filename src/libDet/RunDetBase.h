//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_DET_BASE_H__
#define __RUN_DET_BASE_H__
#include "libUtil/util.h"
#include "libDs/ThreadX.h"
#include "libDs/Yuv420Frm_h.h"
#include "libDs/RgbFrm_h.h"
#include "DetDefs.h"
namespace xeyes {
	class DET_EXPORT RunDetBase : public ThreadX
	{
	public:
		RunDetBase( const int camId=0, const int threadId=0, const std::string &threadName="unkn");
		RunDetBase( const RunDetBase &x) = delete;
		virtual ~RunDetBase();

	protected:
		//set <isEmitSignal> as false when doing single thread none GUI test
		virtual void procNextTask()=0;
		virtual bool procInit()=0;
		void 		 dumpRgbFrm( const std::string &folderPath );

	protected:
		int 			m_camId;
		std::string 	m_camName;	
		uint64			m_frmNum;
		int             m_detPyrL;
		  
		Yuv420Frm_hPtr	m_yuvFrm_h;   //input/captured frm to process
		RgbFrm_hPtr		m_rgbFrm_h;   //output frm after detetcion processing

		DcPtr 			m_camDc;      //shared data container of this camera
	};
	typedef std::shared_ptr<RunDetBase> RunDetBasePtr;
}
#endif // CLIENTGUI_H

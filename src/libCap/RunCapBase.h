//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_CAP_BASE_H__
#define __RUN_CAP_BASE_H__
#include "libUtil/util.h"
#include "libDs/ThreadX.h"
#include "libDs/YuvFrm_h.h"
#include "CapDefs.h"
namespace xeyes {
	class RunDetBase;
	class CAP_EXPORT RunCapBase : public ThreadX
	{
	public:
		RunCapBase( const int camId=0, const int threadId=0, const std::string &threadName="unkn");
		RunCapBase( const RunCapBase &x) = delete;
		virtual ~RunCapBase();
		void setDetPtr( RunDetBase *p );
	protected:
		//set <isEmitSignal> as false when doing single thread none GUI test
		virtual void procNextTask()=0;
		virtual bool procInit()=0;
		void 		 dumpYuvFrm( const std::string &folderPath );

	protected:
		int 			m_camId;
		std::string 	m_camName;	
		uint64			m_frmNum;
		int				m_frmInterval_ms;

		YuvFrm_hPtr	m_yuvFrm_h;   //output/captured frm from this camera
		DcPtr 			m_camDc;      //shared data container of this camera
		CfgCam			m_camCfg;     //hd copy this camera cfg
		RunDetBase*		m_detPtr;
	};
	typedef std::shared_ptr<RunCapBase> RunCapBasePtr;
}
#endif 

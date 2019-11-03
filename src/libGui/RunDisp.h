//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_DISP_H__
#define __RUN_DISP_H__
#include "libUtil/util.h"
#include "libDs/ThreadX.h"
#include "libDs/RgbFrm_h.h"
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
		RgbFrm_hPtr		m_dispFrm_h;    //a frm come from multi-camera
		RgbFrm_hPtr		m_camFrm_h;     //1 frm from once camera
	};
	typedef std::shared_ptr<RunDisp> RunDispPtr;
}
#endif

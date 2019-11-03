//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_CAP_CAM_A_H__
#define __RUN_CAP_CAM_A_H__
#include "libUtil/util.h"
#include "libDs/ThreadX.h"
#include "libDs/Yuv420Frm_h.h"

#include "RunCapBase.h"
namespace xeyes {
	class CAP_EXPORT RunCapCamA : public RunCapBase
	{
	public:
		RunCapCamA(const int camId=0, const int threadId=0, const std::string &threadName="camA_Cap_thread");
		~RunCapCamA();

	protected:
		virtual void procNextTask();
		virtual bool procInit();
	
	protected:
		cv::Mat		m_synBgrFrm;
	};
	typedef std::shared_ptr<RunCapCamA> RunCapCamAPtr;
}
#endif // CLIENTGUI_H

//this is  template class for capture therad, which can be used as a start point to add new cameras
#ifndef __CAP_TREAD_TEMP_H__
#define __CAP_TREAD_TEMP_H__
#include "libUtil/util.h"
#include "libDc/ThreadX.h"
#include "libDc/YuvFrm_h.h"

#include "CapThreadBase.h"
namespace xeyes {
	class CAP_EXPORT CapThreadCamTemp : public CapThreadBase
	{
	public:
		CapThreadCamTemp(const int camId=0, const int threadId=0, const std::string &threadName="camA_Cap_thread");
		virtual ~CapThreadCamTemp();

	protected:
		virtual void procNextTask();
		virtual bool procInit();
	
	protected:
		cv::Mat		m_synBgrFrm;
	};
	typedef std::shared_ptr<CapThreadCamTemp> CapThreadCamTempPtr;
}
#endif 

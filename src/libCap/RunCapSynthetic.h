//capture for synthetic camera
#ifndef __RUN_CAP_SYNTHETIC_A_H__
#define __RUN_CAP_SYNTHETIC_A_H__
#include "libUtil/util.h"
#include "libDs/ThreadX.h"
#include "libDs/YuvFrm_h.h"

#include "RunCapBase.h"
namespace xeyes {
	class CAP_EXPORT RunCapSynthetic : public RunCapBase
	{
	public:
		RunCapSynthetic(const int camId=0, const int threadId=0, const std::string &threadName="camA_Cap_thread");
		virtual ~RunCapSynthetic();

	protected:
		virtual void procNextTask();
		virtual bool procInit();
	
	protected:
		cv::Mat		m_synBgrFrm;
	};
	typedef std::shared_ptr<RunCapSynthetic> RunCapSyntheticPtr;
}
#endif 

//capture for synthetic camera
#ifndef __RUN_CAP_SYNTHETIC_A_H__
#define __RUN_CAP_SYNTHETIC_A_H__
#include "libUtil/util.h"
#include "libDc/ThreadX.h"
#include "libDc/YuvFrm_h.h"

#include "CapThreadBase.h"
namespace xeyes {
	class CAP_EXPORT CapThreadSyn : public CapThreadBase
	{
	public:
		CapThreadSyn(const int camId=0, const int threadId=0, const std::string &threadName="camA_Cap_thread");
		virtual ~CapThreadSyn();

	protected:
		virtual void procNextTask();
		virtual bool procInit();
	
	protected:
		cv::Mat		m_synBgrFrm;
	};
	typedef std::shared_ptr<CapThreadSyn> CapThreadSynPtr;
}
#endif 

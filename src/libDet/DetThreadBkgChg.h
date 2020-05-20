//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __DET_THREAD_BKG_CHG_H__
#define __DET_THREAD_BKG_CHG_H__
#include "DetThreadBase.h"
namespace xeyes {
	class DET_EXPORT DetThreadBkgChg : public DetThreadBase
	{
	public:
		DetThreadBkgChg(const int camId=0, const int threadId=0, const std::string &threadName="det_thread");
		virtual ~DetThreadBkgChg();

	protected:
		virtual void 	procNextTask();
		virtual bool 	procInit();
		bool 			doChgDet();
	};
	typedef std::shared_ptr<DetThreadBkgChg> DetThreadBkgChgPtr;
}
#endif 

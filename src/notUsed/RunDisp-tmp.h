//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_DET_BKG_CHG_H__
#define __RUN_DET_BKG_CHG_H__
#include "RunDetBase.h"
namespace xeyes {
	class DET_EXPORT RunDetBkgChg : public RunDetBase
	{
	public:
		RunDetBkgChg(const int camId=0, const int threadId=0, const std::string &threadName="det_thread");
		~RunDetBkgChg();

	protected:
		virtual void 	procNextTask();
		virtual bool 	procInit();
		bool 			doChgDet();
	};
	typedef std::shared_ptr<RunDetBkgChg> RunDetBkgChgPtr;
}
#endif // CLIENTGUI_H

//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __DET_THREAD_DEEP_NET_H__
#define __DET_THREAD_DEEP_NET_H__

#include "DetThreadBase.h"

//from jetson-inference 
#include "detectNet.h"
#include "cudaMappedMemory.h"

namespace xeyes {
	class DET_EXPORT DetThreadDeepNet : public DetThreadBase
	{
	public:
		DetThreadDeepNet(const int camId=0, const int threadId=0, const std::string &threadName="det_thread");
		virtual ~DetThreadDeepNet();

		static bool rgb2float4(float4* cpuRgba, int cpuImgW, int cpuImgH, const cv::Mat& bgrSrc );

	protected:
		virtual void 	procNextTask();
		virtual bool 	procInit();
		bool 			doChgDet();

	private:
		std::shared_ptr <detectNet> m_deepNet;
		uint32_t					m_overlayFlags;

		//note: both <m_detImgCpu> and <m_detImgGpu> point to the same physical memory area but with differen addresses fro cup and gpu devices
		float4*						m_detImgCpu;         
		float4*						m_detImgGpu;
		int							m_detImgW;
		int							m_detImgH;
	};
	typedef std::shared_ptr<DetThreadDeepNet> DetThreadDeepNetPtr;
}
#endif 

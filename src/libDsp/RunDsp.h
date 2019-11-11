//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __RUN_DSP_H__
#define __RUN_DSP_H__
#include "libUtil/util.h"
#include "libUtil/CvQtUtil.h"
#include "libDs/ThreadX.h"
#include "libDs/DetFrm_h.h"
#include "libDs/DspFrm_h.h"
#include "libDs/YuvFrm_h.h"

#include "ThreadQt.h"
#include "DspDefs.h"

#include <QtCore/QObject>
namespace xeyes {
	class DSP_EXPORT RunDsp : public ThreadQt
	{
		Q_OBJECT
	public:
		RunDsp( const int camId=0, const int threadId=0, const std::string &threadName="unkn");
		RunDsp( const RunDsp &x) = delete;
		virtual ~RunDsp();

		void dumpFrm(const std::string &folderPath);

	signals:
		void sigReady2Disp( const uint64_t fn);

	protected:
		//set <isEmitSignal> as false when doing single thread none GUI test
		virtual void procNextTask();
		virtual bool procInit();
		bool		 prepareDspImg();

	protected:
		int 			m_camId;
		int             m_detPyrL;
		int             m_dspPyrL;
		cv::Size		m_dspSz;

		YuvFrm_hPtr		m_yuvFrm_h;   //input: original frm captured by camera
		DetFrm_hPtr		m_detFrm_h;   //input: detetcion results

		DspFrm_hPtr		m_dspFrm_h;   //ouput: binded original frm with detetcion results
		DcPtr 			m_camDc;      //shared data container of this camera

		//temporal varaibles
		cv::Mat			m_dspImg;
	};
	typedef std::shared_ptr<RunDsp> RunDspPtr;
}
#endif 

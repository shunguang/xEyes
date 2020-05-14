
//--------------------------------------------------------------------------------------
// Objets:
//  1. decode H264 stream via hardward devices
//  2. save the roiginal stream video as mp4 files.
// 
// To keep the unified archeture unchange, for gst-rtsp mainloop kind of caprure, we do:
// 1. frms received by new_frm_cb() are saved in a local queue (redundency, unneccassry)
// 2. procNextTask() moves the avaiable frame into 	m_camDc->m_frmInfoQ;
// by swu on 2/22/2020
//--------------------------------------------------------------------------------------
#ifndef __CAP_SAVE_RTSP_H264_H__
#define __CAP_SAVE_RTSP_H264_H__

#include <cstdlib>
#include <gst/gst.h>
#include <gst/gstinfo.h>
#include <gst/app/gstappsink.h>
#include <glib-unix.h>
#include <dlfcn.h>

#include "libUtil/util.h"
#include "libDc/ThreadX.h"
#include "libDc/YuvFrm_h.h"

#include "CapThreadBase.h"

#define CAP_TO_HOST     1  //0 - does not work (todo:)

namespace xeyes {
	class CAP_EXPORT CapSaveRtspH264 : public CapThreadBase
	{
	public:
		CapSaveRtspH264(const int camId=0, const int threadId=0, const std::string &threadName="camA_Cap_thread");
		virtual ~CapSaveRtspH264();
		virtual void start();
		virtual void forceQuit();

	protected:
		virtual void procNextTask();
		virtual bool procInit();
		void startMainLoopThread();
		void endMainLoopThread();

	protected:
		static void eos_cb(GstAppSink * appsink, gpointer user_data);
		static GstFlowReturn new_sample_cb(GstAppSink *appsink, gpointer user_data);

		int	h264_dec_n_save_loop();
		std::string createLaunchStr();

	protected:
		std::shared_ptr<boost::thread>	gstThread_;
		GMainLoop	*main_loop_;
		GstPipeline *gst_pipeline_;
		bool		 mainLoopExited_;
		bool		 mainLoopThreadRunning_;

		//local 
		YuvCircularQ_h  m_localYuvFrmQ_h;  //accessed by two threads: <gstThread_> and captureThread who calls this->procNextTask();
	};
	typedef std::shared_ptr<CapSaveRtspH264> CapSaveRtspH264Ptr;
}
#endif 

#include <cstdlib>
#include <gst/gst.h>
#include <gst/gstinfo.h>
#include <gst/app/gstappsink.h>
#include <glib-unix.h>
#include <dlfcn.h>

#include <iostream>
#include <sstream>
#include <thread>

//capture to host or device
#include "libDc/YuvFrm_h.h"
#include "libDc/YuvFrm_d.h"

#define USE(x) ((void)(x))

 
namespace xeyes {
	class CapAndSaveThread
	{
	public:
		CapAndSaveThread();
		~CapAndSaveThread();
		void start();

        YuvFrm_hPtr g_yuv_h;

    private:
        int main_loop();
        static void appsink_eos(GstAppSink * appsink, gpointer user_data);
        static GstFlowReturn new_buffer(GstAppSink *appsink, gpointer user_data);

    	std::shared_ptr<boost::thread>	gstThread_;

        GstPipeline *gst_pipeline;
        std::string launch_string;   
    };
}



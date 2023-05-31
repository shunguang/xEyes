/*
*------------------------------------------------------------------------
*CapAndSaveThread.h
*
* This code was developed by Shunguang Wu in his spare time. No government
* or any client funds were used.
*
*
* THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
*
* IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTOR BE LIABLE FOR
* ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
* OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
* WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
* LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
* OF THIS SOFTWARE.
*
* Permission to use, copy, modify, distribute, and sell this software and
* its documentation for any purpose is prohibited unless it is granted under
* the author's written notice.
*
* Copyright(c) 2020 by Shunguang Wu, All Right Reserved
*-------------------------------------------------------------------------
*/
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



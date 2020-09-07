/*
*------------------------------------------------------------------------
*test_gst_rtsp_rcvH264_dec.cpp
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
//#include "nvbuf_utils.h"

#include <iostream>
#include <sstream>
#include <thread>

#define CAP_TO_HOST     0 //1-capture to cpu memory, 0-gpu memory
#define CAP_TO_YUV      1 //if 1-YUV, otherwise rgb
//capture to host or device
#if CAP_TO_HOST
#   include "libDc/YuvFrm_h.h"
#   include "libDc/RgbFrm_h.h"
#else
//deos not work yet!
//Todo: refer test3_how_to_copy_NVMM.cpp
#   include "libDc/YuvFrm_d.h"
#   include "libDc/RgbFrm_d.h"
#endif

using namespace std;
using namespace xeyes;

#define USE(x) ((void)(x))

#if CAP_TO_HOST
    static YuvFrm_hPtr g_yuv_h=nullptr;
    static RgbFrm_hPtr g_rgb_h=nullptr;
#else
    static YuvFrm_dPtr g_yuv_d=nullptr;
    static RgbFrm_dPtr g_rgb_d=nullptr;
#endif

static GstPipeline *gst_pipeline = nullptr;
static string launch_string;   


static void appsink_eos(GstAppSink * appsink, gpointer user_data)
{
    printf("app sink receive eos\n");
    //g_main_loop_quit (hpipe->loop);
}

static GstFlowReturn new_buffer(GstAppSink *appsink, gpointer user_data)
{
    static uint64_t fn=0;
    GstSample *sample = NULL;

    g_signal_emit_by_name (appsink, "pull-sample", &sample,NULL);

    if (sample)
    {
        GstBuffer *buffer = NULL;
        GstCaps   *caps   = NULL;
        GstMapInfo map    = {0};

        caps = gst_sample_get_caps (sample);
        if (!caps){
            printf("could not get snapshot format\n");
        }
        gst_caps_get_structure (caps, 0);
        buffer = gst_sample_get_buffer (sample);
        gst_buffer_map (buffer, &map, GST_MAP_READ);

        printf("fn=%llu, map.size = %lu, bufSz=%lu\n", fn, map.size, gst_buffer_get_size(buffer));
        if(fn%100==0){
#if CAP_TO_HOST
#   if CAP_TO_YUV
            assert( g_yuv_h->sz_ == map.size );
            g_yuv_h->hdCopyFromBuf( map.data, map.size, fn);
            g_yuv_h->dump(".", "yuv_h");
#   else
            //g_rgb_h->hdCopyFromBuf(map.data, map.size, fn);
            //g_rgb_h->dump(".", "rgb_h");
#   endif
#else            
            //myAssert( g_yuv_d->sz_ == map.size. "size doe not match!" );
            cout << "yuvSz(w=" << g_yuv_d->size_.width << ",h=" << g_yuv_d->size_.height << ")" <<endl;
            //g_yuv_d->hdCopyFromHostBuf( map.data, map.size, fn);
            //g_yuv_d->dump(".", "yuv_d");
#endif
        }

        gst_buffer_unmap(buffer, &map);
        gst_sample_unref (sample);
        ++fn;
    }
    else
    {
        g_print ("could not make snapshot\n");
    }

    return GST_FLOW_OK;
}

int test_gst_rtsp_rcvH264_dec(int argc, char** argv) {
    USE(argc);
    USE(argv);

    gst_init (&argc, &argv);

    GMainLoop *main_loop;
    main_loop = g_main_loop_new (NULL, FALSE);
    ostringstream launch_stream;
    int w = 1920;
    int h = 1080;
#if CAP_TO_HOST
    g_yuv_h.reset( new YuvFrm_h(w,h) ) ;
    g_rgb_h.reset( new RgbFrm_h(w,h) ) ;
#else
    g_yuv_d.reset( new YuvFrm_d(w,h) ) ;
    g_rgb_d.reset( new RgbFrm_d(w,h) ) ;
#endif

    //I420, UYVY, YUY2, YVYU, NV12, GRAY8, BGRx,and RGBA 
#if CAP_TO_YUV
    string fmtStr="I420";
#else
    string fmtStr="RGBA";  
#endif

    GstAppSinkCallbacks callbacks = {appsink_eos, NULL, new_buffer};

    bool decOnly = true;
    GError *error = nullptr;
    if( decOnly ){
        //gst-launch-1.0 rtspsrc location="rtsp://192.168.1.5/11" ! rtph264depay ! h264parse ! omxh264dec ! 
        //nveglglessink window-x=100 window-y=100 window-width=640 window-height=360
        launch_stream
        << "rtspsrc  location= rtsp://192.168.1.5/11 ! "
        << "rtph264depay ! h264parse ! omxh264dec ! "
#if CAP_TO_HOST    
        << "nvvidconv ! "      
        << "video/x-raw, format=" << fmtStr << ", width="<< w <<", height="<< h <<" ! "
#else
        << "nvvidconv ! "
        << "video/x-raw(memory:NVMM), format=" << fmtStr << ", width="<< w <<", height="<< h <<" ! "
#endif
        << "appsink name=mysink ";
   
        launch_string = launch_stream.str();
        g_print("Using launch string: %s\n", launch_string.c_str());

        gst_pipeline  = (GstPipeline*) gst_parse_launch(launch_string.c_str(), &error);
        if (gst_pipeline == nullptr) {
            g_print( "Failed to parse launch: %s\n", error->message);
            return -1;
        }
        if(error) g_error_free(error);

        GstElement *appsink_ = gst_bin_get_by_name(GST_BIN(gst_pipeline), "mysink");
        gst_app_sink_set_callbacks (GST_APP_SINK(appsink_), &callbacks, NULL, NULL);

        gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_PLAYING); 
    }
    else{
        //gst-launch-1.0 -e rtspsrc location="rtsp://192.168.1.5/11" 
        // ! rtph264depay ! h264parse ! splitmuxsink location=file-%03d.mp4
        launch_stream
        << "-e rtspsrc  location= rtsp://192.168.1.5/11 ! "
        << "rtph264depay ! h264parse ! splitmuxsink location=file-%03d.mp4";
   
        launch_string = launch_stream.str();
        g_print("Using launch string: %s\n", launch_string.c_str());

        gst_pipeline  = (GstPipeline*) gst_parse_launch(launch_string.c_str(), &error);
        if (gst_pipeline == nullptr) {
            g_print( "Failed to parse launch: %s\n", error->message);
            return -1;
        }
        if(error) g_error_free(error);
        gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_PLAYING); 
    }

    sleep(10);
    g_main_loop_run (main_loop);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(gst_pipeline));
    g_main_loop_unref(main_loop);

    g_print("going to exit \n");
    return 0;
}

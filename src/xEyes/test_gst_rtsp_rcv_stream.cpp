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

using namespace std;
using namespace xeyes;

#define USE(x) ((void)(x))
#define CAP_TO_HOST     1  //todo: 0-does not work
#if CAP_TO_HOST
    static YuvFrm_hPtr g_yuv_h=nullptr;
#else
    static YuvFrm_dPtr g_yuv_d=nullptr;
#endif

static GstPipeline *gst_pipeline = nullptr;
static string launch_string;   


static void appsink_eos(GstAppSink * appsink, gpointer user_data)
{
    printf("app sink receive eos\n");
//    g_main_loop_quit (hpipe->loop);
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
        if (!caps)
        {
            printf("could not get snapshot format\n");
        }
        gst_caps_get_structure (caps, 0);
        buffer = gst_sample_get_buffer (sample);
        gst_buffer_map (buffer, &map, GST_MAP_READ);

        if(fn%100==0){
#if CAP_TO_HOST
            assert( g_yuv_h->sz_ == map.size );
            g_yuv_h->hdCopyFromBuf( map.data, map.size, fn);
            g_yuv_h->dump(".", "yuv_h");
#else            
            assert( g_yuv_d->sz_ == map.size );
            g_yuv_d->hdCopyFromHostBuf( map.data, map.size, fn);
            g_yuv_d->dump(".", "yuv_d");
#endif
            printf("fn=%llu, map.size = %lu, bufSz=%lu\n", fn, map.size, gst_buffer_get_size(buffer));
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

int test_gst_rtsp_rcv_stream(int argc, char** argv) {
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
#else
    g_yuv_d.reset( new YuvFrm_d(w,h) ) ;
#endif
    
    GstAppSinkCallbacks callbacks = {appsink_eos, NULL, new_buffer};

    //gst-launch-1.0 rtspsrc location="rtsp://192.168.1.5/11" ! rtph264depay ! h264parse ! omxh264dec ! 
    //nveglglessink window-x=100 window-y=100 window-width=640 window-height=360
#if 0
    launch_stream
    << "nvcamerasrc ! "
    << "video/x-raw(memory:NVMM), width="<< w <<", height="<< h <<", framerate=30/1 ! " 
    << "nvvidconv ! "
    << "video/x-raw, format=I420, width="<< w <<", height="<< h <<" ! "
    << "appsink name=mysink ";
#else    
    //launch_string = "gst-launch-1.0 rtspsrc location="rtsp://192.168.1.5/11" ! rtph264depay ! h264parse ! omxh264dec ! 
    //nveglglessink window-x=100 window-y=100 window-width=640 window-height=360
   
    launch_stream
    << "rtspsrc  location= rtsp://192.168.1.5/11 ! "
    << "rtph264depay ! h264parse ! omxh264dec ! "
#if CAP_TO_HOST    
    << "nvvidconv ! "
    << "video/x-raw, format=I420, width="<< w <<", height="<< h <<" ! "
#else
    << "video/x-raw(memory:NVMM), format=I420, width="<< w <<", height="<< h <<" ! "
#endif
    << "appsink name=mysink ";
#endif
    launch_string = launch_stream.str();

    g_print("Using launch string: %s\n", launch_string.c_str());

    GError *error = nullptr;
    gst_pipeline  = (GstPipeline*) gst_parse_launch(launch_string.c_str(), &error);

    if (gst_pipeline == nullptr) {
        g_print( "Failed to parse launch: %s\n", error->message);
        return -1;
    }
    if(error) g_error_free(error);

    GstElement *appsink_ = gst_bin_get_by_name(GST_BIN(gst_pipeline), "mysink");
    gst_app_sink_set_callbacks (GST_APP_SINK(appsink_), &callbacks, NULL, NULL);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_PLAYING); 

    sleep(10);
    g_main_loop_run (main_loop);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(gst_pipeline));
    g_main_loop_unref(main_loop);

    g_print("going to exit \n");
    return 0;
}
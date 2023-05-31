/*
*------------------------------------------------------------------------
*CapAndSaveThread.cpp
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
#include "CapAndSaveThread.h"

using namespace std;
using namespace xeyes;
CapAndSaveThread::CapAndSaveThread()
	:gstThread_(nullptr)
    ,g_yuv_h(nullptr)
    ,gst_pipeline(nullptr)
    ,launch_string("")   
{
}

CapAndSaveThread::~CapAndSaveThread()
{

}

void CapAndSaveThread::start(){
	dumpLog( "CapAndSaveThread::start(): start captures" );
	gstThread_.reset(new boost::thread(boost::bind(&CapAndSaveThread::main_loop, this) ) );
}


void CapAndSaveThread::appsink_eos(GstAppSink * appsink, gpointer user_data)
{
    printf("app sink receive eos\n");
    CapAndSaveThread *pThis = reinterpret_cast<CapAndSaveThread*>(user_data);
    printf("appsink_eos():  w=%d\n", pThis->g_yuv_h->w_ );
//    g_main_loop_quit (hpipe->loop);
}

GstFlowReturn CapAndSaveThread::new_buffer(GstAppSink *appsink, gpointer user_data)
{
    static uint64_t fn=0;
    GstSample *sample = NULL;


    CapAndSaveThread *pThis = reinterpret_cast<CapAndSaveThread*>(user_data);
    //printf("new_buffer(): fn=%llu,  w=%d\n", fn, pThis->g_yuv_h->w_ );

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

        if(fn%1000==0){
            assert( pThis->g_yuv_h->sz_ == map.size );
            pThis->g_yuv_h->hdCopyFromBuf( map.data, map.size, fn);
            pThis->g_yuv_h->dump(".", "yuv_h");
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

int CapAndSaveThread::main_loop() {
    //USE(argc);
    //USE(argv);

    gst_init ( NULL, NULL);

    GMainLoop *main_loop;
    main_loop = g_main_loop_new (NULL, FALSE);
    ostringstream launch_stream;
    int w = 1920;
    int h = 1080;
    g_yuv_h.reset( new YuvFrm_h(w,h) ) ;
    
    GstAppSinkCallbacks callbacks = { CapAndSaveThread::appsink_eos, NULL, CapAndSaveThread::new_buffer};

    //rtph264depay-- Extracts H264 video from RTP packets
    //splitmuxsink:
    //max-size-bytes: max. amount of data per file (in bytes, 0=disable).
    //max-size-time:  max. amount of time per file (in ns, 0=disable).
    //max-files:      maximum number of files to keep on disk. Once the maximum is reached,old files start to be deleted to make room for new ones.
    //send-keyframe-requests: request a keyframe every max-size-time ns to try splitting at that point. Needs max-size-bytes to be 0 in order to be effective.

    launch_stream
    << "-e -v rtspsrc  location= rtsp://192.168.1.5/11 ! "
    << "tee name=tsplit ! "     //split into two parts
    << "queue ! rtph264depay ! h264parse ! "   //Parses H.264 streams  
    << "omxh264dec ! "          //hd decoder
    << "nvvidconv ! "
    << "video/x-raw, format=I420, width="<< w <<", height="<< h <<" ! "
    << "appsink name=myYuvSink tsplit. ! "
    << "queue ! rtph264depay ! h264parse ! "
    << "splitmuxsink name=myMp4Sink location=./frontDoor/test_save_%04d.mp4 max-size-time=7200000000000 max-size-bytes=0 max-files=100 send-keyframe-requests=TRUE";

    launch_string = launch_stream.str();
    g_print("Using launch string: %s\n", launch_string.c_str());

    GError *error = nullptr;
    gst_pipeline  = (GstPipeline*) gst_parse_launch(launch_string.c_str(), &error);

    if (gst_pipeline == nullptr) {
        g_print( "Failed to parse launch: %s\n", error->message);
        return -1;
    }
    if(error) g_error_free(error);

    GstElement *yuvSink_ = gst_bin_get_by_name(GST_BIN(gst_pipeline), "myYuvSink");
    gst_app_sink_set_callbacks (GST_APP_SINK(yuvSink_), &callbacks, this, NULL);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_PLAYING); 

    sleep(10);
    dumpLog( "g_main_loop_run() ... " );   
    g_main_loop_run (main_loop);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(gst_pipeline));
    g_main_loop_unref(main_loop);

    g_print("going to exit \n");
    return 0;
}

#if 0
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

        if(fn%2000==0){
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

int test_gst_rtsp_rcvH264_dec_and_save(int argc, char** argv) {
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

    //rtph264depay-- Extracts H264 video from RTP packets
    //splitmuxsink:
    //max-size-bytes: max. amount of data per file (in bytes, 0=disable).
    //max-size-time:  max. amount of time per file (in ns, 0=disable).
    //max-files:      maximum number of files to keep on disk. Once the maximum is reached,old files start to be deleted to make room for new ones.
    //send-keyframe-requests: request a keyframe every max-size-time ns to try splitting at that point. Needs max-size-bytes to be 0 in order to be effective.

    launch_stream
    << "-e -v rtspsrc  location= rtsp://192.168.1.5/11 ! "
    << "tee name=tsplit ! "     //split into two parts
    << "queue ! rtph264depay ! h264parse ! "   //Parses H.264 streams  
    << "omxh264dec ! "          //hd decoder
    << "nvvidconv ! "
    << "video/x-raw, format=I420, width="<< w <<", height="<< h <<" ! "
    << "appsink name=myYuvSink tsplit. ! "
    << "queue ! rtph264depay ! h264parse ! "
    << "splitmuxsink name=myMp4Sink location=./frontDoor/test_save_%04d.mp4 max-size-time=7200000000000 max-size-bytes=0 max-files=100 send-keyframe-requests=TRUE";

    launch_string = launch_stream.str();
    g_print("Using launch string: %s\n", launch_string.c_str());

    GError *error = nullptr;
    gst_pipeline  = (GstPipeline*) gst_parse_launch(launch_string.c_str(), &error);

    if (gst_pipeline == nullptr) {
        g_print( "Failed to parse launch: %s\n", error->message);
        return -1;
    }
    if(error) g_error_free(error);

    GstElement *yuvSink_ = gst_bin_get_by_name(GST_BIN(gst_pipeline), "myYuvSink");
    gst_app_sink_set_callbacks (GST_APP_SINK(yuvSink_), &callbacks, NULL, NULL);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_PLAYING); 

    sleep(10);
    g_main_loop_run (main_loop);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(gst_pipeline));
    g_main_loop_unref(main_loop);

    g_print("going to exit \n");
    return 0;
}
#endif

//https://devtalk.nvidia.com/default/topic/1037450/jetson-tx2/use-gstreamer-or-tegra_multimedia_api-to-decode-video-would-be-more-efficient-and-increase-throughpu-/post/5270860/#5270860


#include <cstdlib>
#include <gst/gst.h>
#include <gst/gstinfo.h>
#include <gst/app/gstappsink.h>
#include <glib-unix.h>
#include <dlfcn.h>

#include <iostream>
#include <sstream>
#include <thread>

#include "NvEglRenderer.h"
#include "nvbuf_utils.h"

using namespace std;

#define USE(x) ((void)(x))

static GstPipeline *gst_pipeline = nullptr;
static string launch_string;
static int frame_count = 0;
static int sleep_count = 0;
static int eos = 0;
static NvEglRenderer *renderer;

static void appsink_eos(GstAppSink * appsink, gpointer user_data)
{
    printf("app sink receive eos\n");
    eos = 1;
//    g_main_loop_quit (hpipe->loop);
}

static GstFlowReturn new_buffer(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = NULL;

    g_signal_emit_by_name (appsink, "pull-sample", &sample,NULL);

    if (sample)
    {
        GstBuffer *buffer = NULL;
        GstCaps   *caps   = NULL;
        GstMapInfo map    = {0};
        int dmabuf_fd = 0;

        caps = gst_sample_get_caps (sample);
        if (!caps)
        {
            printf("could not get snapshot format\n");
        }
        gst_caps_get_structure (caps, 0);
        buffer = gst_sample_get_buffer (sample);
        gst_buffer_map (buffer, &map, GST_MAP_READ);

        ExtractFdFromNvBuffer((void *)map.data, &dmabuf_fd);
        //printf("dmabuf_fd %d \n", dmabuf_fd);
        renderer->render(dmabuf_fd);
        NvReleaseFd(dmabuf_fd);
        frame_count++;

        gst_buffer_unmap(buffer, &map);

        gst_sample_unref (sample);
    }
    else
    {
        g_print ("could not make snapshot\n");
    }

    return GST_FLOW_OK;
}

int main(int argc, char** argv) {
    USE(argc);
    USE(argv);

    gst_init (&argc, &argv);

    GMainLoop *main_loop;
    main_loop = g_main_loop_new (NULL, FALSE);
    ostringstream launch_stream;
    int w = 1920;
    int h = 816;
    GstAppSinkCallbacks callbacks = {appsink_eos, NULL, new_buffer};

    launch_stream
//    << "nvcamerasrc ! "
//    << "video/x-raw(memory:NVMM), width="<< w <<", height="<< h <<", framerate=30/1 ! " 
    << "filesrc location=Bourne_Trailer.mp4 ! decodebin ! "
    << "nvvidconv ! "
    << "video/x-raw(memory:NVMM), format=I420, width="<< w <<", height="<< h <<" ! "
    << "appsink name=mysink ";

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

    renderer = NvEglRenderer::createEglRenderer("renderer0",
                w, h, 0, 0);
    renderer->setFPS(24);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_PLAYING); 

    while (eos == 0) {
        sleep(1);
        sleep_count++;
    }
    //sleep(90);
    //g_main_loop_run (main_loop);

    gst_element_set_state((GstElement*)gst_pipeline, GST_STATE_NULL);
    gst_object_unref(GST_OBJECT(gst_pipeline));
    g_main_loop_unref(main_loop);

    delete renderer;

    g_print("going to exit, decode %d frames in %d seconds \n", frame_count, sleep_count);
    return 0;
}

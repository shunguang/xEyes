/*
*------------------------------------------------------------------------
*test_gst_rtsp_sndH264_stream.cpp
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
/* GStreamer
 * Copyright (C) 2008 Wim Taymans <wim.taymans at gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include <stdio.h>
#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>
#include "libUtil/util.h"
#include "libDc/YuvFrm.h"
#include "libThreadServer/CudaImgs.h"
#include "libThreadServer/YuvCircularQ_dev.h"

/*
<item>0, 7936, 6016, 800000< / item>
<item>1, 3968, 3008, 240000< / item>
<item>2, 1984, 1504, 135000< / item>
<item>3, 992, 752,   20000< / item>
<item>4, 496, 376,   10000< / item>
<item>5, 248, 188,   768< / item>
*/

#define MY_IMG_W  3968
#define MY_IMG_H  3008
#define MY_FPS    25
#define FEED_FROM_HOST_IMG 1
#define GBUFFF_ALLOCATE_MEM 0

using namespace cv;
using namespace ngv;

typedef struct
{
	GstClockTime timestamp;
} MyContext;

//global smart pointers, memory allocated only once outside the loop
YuvCircularQ_devPtr		g_devYuvQ;          //YUV image queque
YuvFrmPtr				g_yuv_host;			//YUV frame img at host
CudaYuv420pImgPtr		g_yuv_dev;			//YUV frame img at dev
boost::posix_time::ptime  g_preTime;

void inti_globals(int qSize);
/* called when we need to give data to appsrc */
static void need_data(GstElement *appsrc, guint unused, MyContext *ctx)
{
	static int fn = 0;
	boost::posix_time::ptime t0 = POSIX_LOCAL_TIME;
	GstFlowReturn ret;

#if FEED_FROM_HOST_IMG
	//read next img in <g_devYuvQ> to <g_yuv_host>
	g_devYuvQ->readNext(g_yuv_host, false);    
#if GBUFFF_ALLOCATE_MEM
	//copy <g_yuv_host> to <buffer>
	gsize size = MY_IMG_W * MY_IMG_H * 3 / 2;
	GstBuffer *buffer = gst_buffer_new_allocate(NULL, size, NULL);
	gst_buffer_fill(buffer, 0, (gconstpointer)g_yuv_host->buf_, gsize(g_yuv_host->sz_));
#else
	GstBuffer *buffer = gst_buffer_new_wrapped( (guchar *)g_yuv_host->buf_, gsize(g_yuv_host->sz_));
#endif

	/* increment the timestamp every 1/MY_FPS second */
	GST_BUFFER_PTS(buffer) = ctx->timestamp;
	GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND, MY_FPS);
	ctx->timestamp += GST_BUFFER_DURATION(buffer);

	g_signal_emit_by_name(appsrc, "push-buffer", buffer, &ret);

#	if GBUFFF_ALLOCATE_MEM
	gst_buffer_unref(buffer);
#	endif

#else  //feed from dev
	g_devYuvQ->readNext(g_yuv_dev, false);
	//??????????????????????????????????????????????????????????????????
	//at this poitn image is already stored in device memeory <g_yuv_dev>, how to feed it into appsrc
	//??????????????????????????????????????????????????????????????????
#endif

	if (fn % 100 == 0)
		printf("fn=%d, dt1=%ld(ms), dt2=%d\n", fn, ngv::timeIntervalMillisec(t0), ngv::timeIntervalMillisec(g_preTime));

	g_preTime = POSIX_LOCAL_TIME;
	++fn;
}

/* called when a new media pipeline is constructed. We can query the
 * pipeline and configure our appsrc */
static void
media_configure(GstRTSPMediaFactory *factory, GstRTSPMedia *media, gpointer user_data)
{
	GstElement *element, *appsrc;
	MyContext *ctx;

	/* get the element used for providing the streams of the media */
	element = gst_rtsp_media_get_element(media);

	/* get our appsrc, we named it 'mysrc' with the name property */
	appsrc = gst_bin_get_by_name_recurse_up(GST_BIN(element), "mysrc");

	/* this instructs appsrc that we will be dealing with timed buffer */
	gst_util_set_object_arg(G_OBJECT(appsrc), "format", "time");
	/* configure the caps of the video */
	g_object_set(G_OBJECT(appsrc), "caps",
#if FEED_FROM_HOST_IMG
		gst_caps_new_simple("video/x-raw",
#else
		gst_caps_new_simple("video/x-raw (memory:NVMM)",
#endif
			"format", G_TYPE_STRING, "I420",
			"width", G_TYPE_INT, MY_IMG_W,
			"height", G_TYPE_INT, MY_IMG_H,
			"framerate", GST_TYPE_FRACTION, MY_FPS, 1,
			NULL),
		NULL);

	ctx = g_new0(MyContext, 1);
	ctx->timestamp = 0;
	/* make sure ther datais freed when the media is gone */
	g_object_set_data_full(G_OBJECT(media), "my-extra-data", ctx,
		(GDestroyNotify)g_free);

	/* install the callback that will be called when a buffer is needed */
	g_signal_connect(appsrc, "need-data", (GCallback)need_data, ctx);
	gst_object_unref(appsrc);
	gst_object_unref(element);
}

int test_appsrc_v2(int argc, char *argv[])
{
	//inti global smart pointers
	inti_globals(100);

	GMainLoop *loop;
	GstRTSPServer *server;
	GstRTSPMountPoints *mounts;
	GstRTSPMediaFactory *factory;

	gst_init(&argc, &argv);

	loop = g_main_loop_new(NULL, FALSE);

	/* create a server instance */
	server = gst_rtsp_server_new();

	/* get the mount points for this server, every server has a default object
	 * that be used to map uri mount points to media factories */
	mounts = gst_rtsp_server_get_mount_points(server);

	/* make a media factory for a test stream. The default media factory can use
	 * gst-launch syntax to create pipelines.
	 * any launch line works as long as it contains elements named pay%d. Each
	 * element with pay%d names will be a stream */

	 /*
	  * Once you have a full pipeline, you can replace x264enc with omxh264enc/nvv4l2h264enc to leverage Xavier HW encoder.
	  */

	factory = gst_rtsp_media_factory_new();
	gst_rtsp_media_factory_set_launch(factory, "( appsrc name=mysrc ! omxh264enc ! rtph264pay name=pay0 pt=96 )");

	/* notify when our media is ready, This is called whenever someone asks for
	 * the media and a new pipeline with our appsrc is created */
	g_signal_connect(factory, "media-configure", (GCallback)media_configure, NULL);

	/* attach the test factory to the /test url */
	gst_rtsp_mount_points_add_factory(mounts, "/test", factory);

	/* don't need the ref to the mounts anymore */
	g_object_unref(mounts);

	/* attach the server to the default maincontext */
	gst_rtsp_server_attach(server, NULL);

	/* start serving */
	g_print("stream ready at rtsp://127.0.0.1:8554/test\n");
	g_main_loop_run(loop);

	return 0;
}

void inti_globals(int qSize )
{
	g_preTime = POSIX_LOCAL_TIME;
	g_yuv_host.reset(new YuvFrm(MY_IMG_W, MY_IMG_H));
	g_yuv_dev.reset(new CudaYuv420pImg());
	g_yuv_dev->create(MY_IMG_W, MY_IMG_H);

	std::string f = "../data/initScreen.png";
	cv::Mat I1, I0 = cv::imread(f);
	cv::resize(I0, I1, cv::Size(MY_IMG_W, MY_IMG_H), 0, 0, CV_INTER_LINEAR);

	g_devYuvQ.reset(new YuvCircularQ_dev(MY_IMG_W, MY_IMG_H, qSize));
	YuvFrmPtr yuv( new YuvFrm(MY_IMG_W, MY_IMG_H) );
	const std::string s0 = std::string("RTSP image size=[") + std::to_string(MY_IMG_W) + "," + std::to_string(MY_IMG_H) + "], fn=";
	for (int i = 0; i < qSize; ++i) {
		I1.copyTo(I0);
		cv::putText(I0, s0+std::to_string(i), cv::Point(10, MY_IMG_H - MY_IMG_H/50), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(255, 255, 255), 3);

		yuv->fromBGR(I0, i);
		g_devYuvQ->wrtNext( yuv );
	}
}

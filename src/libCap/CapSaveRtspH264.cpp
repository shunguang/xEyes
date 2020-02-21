#include "libDet/DetThreadBase.h"
#include "CapSaveRtspH264.h"

using namespace std;
using namespace xeyes;
CapSaveRtspH264::CapSaveRtspH264( const int camId, const int threadId, const std::string &threadName )
: CapThreadBase( camId, threadId, threadName )
, main_loop_(nullptr)
, gst_pipeline_(nullptr)
, mainLoopExited_( true )
, mainLoopThreadRunning_(false)
, m_localYuvFrmQ_h();
{
	cout << "CapSaveRtspH264::CapSaveRtspH264(): called" << endl;
}

CapSaveRtspH264::~CapSaveRtspH264()
{
}

//only need to one frame 
//todo: compbine with RunThreadS::runLoop() for efficency 
void CapSaveRtspH264::procNextTask()
{
	const boost::posix_time::ptime start = APP_LOCAL_TIME;

	//read from local <m_localYuvFrmQ_h>
	bool hasFrm = m_localYuvFrmQ_h.read(m_yuvFrm_h.get());
	if (hasFrm) {
		//wrt into global shared data container:  <m_camDc->m_frmInfoQ> to make the pipe line work
		m_camDc->m_frmInfoQ->wrtYuvFrmByCapThread(m_yuvFrm_h.get());
		//wakeup the consumer thread
		m_detPtr->wakeupToWork();
	}

	//decide if need to sleep
	uint32_t dt = timeIntervalMillisec(start);
	if (dt < m_frmInterval_ms) {
		THREAD_SLEEP(m_frmInterval_ms - dt);
	}

	if (m_frmNum % m_frmFreqToLog == 0) {
		dumpLog( "CapSaveRtspH264::procNextTask(): %s, fn=%lld, dt=%d", m_threadName.c_str(), m_frmNum, dt);
	}

	//---- for next frm ------------
	m_frmNum++;
}

bool CapSaveRtspH264::procInit()
{
	//init this camera
	m_camDc = m_dcUI->m_dcMap[m_camId];
	m_camCfg = m_cfg->getCam( m_camId );
	const int &w = m_camCfg.imgSz_.w;
	const int &h = m_camCfg.imgSz_.h;
	std::string recDir = m_camCfg.getRecFolder();

	//init currrent camera capture params
	m_frmInterval_ms = floor( (float)(1000.0/m_camCfg.fps_.getFps()) );

#if CAP_TO_HOST
	m_yuvFrm_h.reset(new YuvFrm_h(w, h));
	
	m_localYuvFrmQ_h.resetName( "CapLocalQue4" + m_camCfg.cameraName_ );
	m_localYuvFrmQ_h.resetSize( w, h, 10);
#else
	m_yuvFrm_d.reset(new YuvFrm_d(w, h));
#endif


	//dumpLog( "Yuv: (w=%d,h=%d)", m_yuvFrm_h->w_, m_yuvFrm_h->h_);

	//check if recoder folder exist
	if (!folderExists(recDir)) {
		myCreateDir(recDir);
	}

	return true;
}


void CapSaveRtspH264::appsink_eos_cb(GstAppSink * appsink, gpointer user_data)
{
	//gateway to access this->xyz
	CapSaveRtspH264 *pThis = reinterpret_cast<CapSaveRtspH264*>(user_data);

	dumpLog("CapSaveRtspH264::appsink_eos_cb(): app sink receive eos!");
	//force main loop quit and starover
	//todo:
	//g_main_loop_quit (hpipe->loop);
}

//-----------------------------------------------------------------------
//wrt decoded frm image into <m_localYuvFrmQ_h>
// or
//<m_localYuvFrmQ_d> if choose  gpu memory
//-----------------------------------------------------------------------
GstFlowReturn CapSaveRtspH264::new_frm_cb(GstAppSink *appsink, gpointer user_data)
{
	//gateway to access this->xyz
	CapSaveRtspH264 *pThis = reinterpret_cast<CapSaveRtspH264*>(user_data);

	GstSample *sample = NULL;
	g_signal_emit_by_name(appsink, "pull-sample", &sample, NULL);
	if (sample){
		GstBuffer *buffer = NULL;
		GstCaps   *caps = NULL;
		GstMapInfo map = { 0 };

		caps = gst_sample_get_caps(sample);
		if (!caps){
			dumpLog("CapSaveRtspH264::new_frm_cb(): could not get snapshot format.");
		}
		gst_caps_get_structure(caps, 0);
		buffer = gst_sample_get_buffer(sample);
		gst_buffer_map(buffer, &map, GST_MAP_READ);

#if CAP_TO_HOST

#if 1	
		//local dump and debug 
		myAssert(pThis->m_yuvFrm_h->sz_ == map.size, "CapSaveRtspH264::new_frm_cb(): size does not match!");
		YuvFrm_h yuv(pThis->m_camCfg.imgSz_.w, pThis->m_camCfg.imgSz_.h);
		yuv.hdCopyFromBuf(map.data, map.size, 0);
		yuv.dump(".", "yuv_h");
#endif

		m_localYuvFrmQ_h->wrt(map.data, map.size, 0);

#else            
		myAssert(g_yuv_d->sz_ == map.size, "CapSaveRtspH264::new_frm_cb(): size does not match!");
		g_yuv_d->hdCopyFromHostBuf(map.data, map.size, 0);
		g_yuv_d->dump(".", "yuv_d");
#endif

		if (pThis->m_frmNum % pThis->m_frmFreqToLog == 0) {
			dumpLog("CapSaveRtspH264::new_frm_cb(): map.size = %lu, bufSz=%lu", map.size, gst_buffer_get_size(buffer));
		}
		gst_buffer_unmap(buffer, &map);
		gst_sample_unref(sample);
	}

	else{
		dumpLog("CapSaveRtspH264::new_frm_cb(): could not make snapshot");
	}

	return GST_FLOW_OK;
}

int CapSaveRtspH264 :: h264_dec_n_save_loop() 
{
	mainLoopExited_ = false;

	gst_init( NULL, NULL);
	main_loop_ = g_main_loop_new(NULL, FALSE);
	GstAppSinkCallbacks callbacks = { appsink_eos_cb, NULL, new_frm_cb };

	string launch_string = createLaunchStr();
	dumpLog("CapSaveRtspH264 :: main_loop(): Using launch string: %s", launch_string.c_str());

	GError *error = nullptr;
	gst_pipeline_ = (GstPipeline*)gst_parse_launch(launch_string.c_str(), &error);

	if (gst_pipeline == nullptr) {
		myExit("CapSaveRtspH264 :: main_loop(): Failed to parse launch: %s", error->message);
	}
	if (error) g_error_free(error);

	GstElement *yuvSink = gst_bin_get_by_name(GST_BIN(gst_pipeline_), "appYuvSink");

	//void gst_app_sink_set_callbacks(GstAppSink *appsink, GstAppSinkCallbacks *callbacks, gpointer user_data, GDestroyNotify notify); ...
	gst_app_sink_set_callbacks(GST_APP_SINK(yuvSink), &callbacks, this, NULL);

	gst_element_set_state((GstElement*)gst_pipeline_, GST_STATE_PLAYING);

	sleep(10);
	dumpLog("CapSaveRtspH264 :: h264_dec_n_save_loop(): %s -- dec and save start!",  m_camCfg.rtspUrl_.c_str() );
	g_main_loop_run(main_loop_);

	//---- reach the next line when quit from g_main_loop_run()
	gst_element_set_state((GstElement*)gst_pipeline_, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT(gst_pipeline_));
	g_main_loop_unref(main_loop_);
	mainLoopExited_ = true;
	dumpLog("CapSaveRtspH264 :: h264_dec_n_save_loop(): % -- dec and save exit!", m_camCfg.rtspUrl_.c_str() );

	return 0;
}

std::string CapSaveRtspH264::createLaunchStr()
{
	//rtph264depay-- Extracts H264 video from RTP packets
	//splitmuxsink:
	//max-size-bytes: max. amount of data per file (in bytes, 0=disable).
	//max-size-time:  max. amount of time per file (in ns, 0=disable).
	//max-files:      maximum number of files to keep on disk. Once the maximum is reached,old files start to be deleted to make room for new ones.
	//send-keyframe-requests: request a keyframe every max-size-time ns to try splitting at that point. Needs max-size-bytes to be 0 in order to be effective.

	ostringstream launchStream;
	launchStream << "-e -v rtspsrc  location= " << m_camCfg.rtspUrl_ << " "
		<< "tee name=tsplit ! "     //split into two parts
		<< "queue ! rtph264depay ! h264parse ! "   //Parses H.264 streams  
		<< "omxh264dec ! "          //hd decoder
		<< "nvvidconv ! "
		<< "video/x-raw, format=I420, width=" << m_camCfg.imgSz_.w << ", height=" << m_camCfg.imgSz_.h << " ! "
		<< "appsink name=appYuvSink tsplit. ! "
		<< "queue ! rtph264depay ! h264parse ! "
		<< "splitmuxsink name=myMp4Sink location=" << m_camCfg.mp4LocationAndPrefix_ << "%04d.mp4 max-size-time=7200000000000 max-size-bytes=0 max-files=100 send-keyframe-requests=TRUE";
	
	return launchStream.str();
}

void CapSaveRtspH264::startMainLoopThread()
{
	dumpLog( "CapSaveRtspH264::startMainLoop(): start capture: %s", m_camCfg.cameraName_.c_str() );
	gstThread_.reset(new boost::thread(&CapSaveRtspH264::h264_dec_n_save_loop, this));
	mainLoopThreadRunning_ = true;
}

void CapSaveRtspH264::endMainLoopThread()
{
	if (!mainLoopThreadRunning_) {
		return;
	}

	dumpLog("CapSaveRtspH264::endMainLoop(): AA- m_mainLoopEnd=%d", mainLoopExited_);
	g_main_loop_quit(main_loop_);
	while (1) {
		BOOST_SLEEP(10);
		if (!g_main_loop_is_running(main_loop_)) {
			break;
		}
#if 0
		//add junk frm
		m_yuvQue_dev->addJunkFrm();
		m_pCW->go_wakeup_one();
#endif
		dumpLog("GstRtspStreamer::endStreamThread(): add junk frm, m_mainLoopEnd=%d", mainLoopExited_);

	}
	gstThread_ -> join();
	mainLoopThreadRunning_ = false;
	dumpLog("GstRtspStreamer::endStreamThread(): BB-m_mainLoopExit=%d, %s", mainLoopExited_, m_camCfg.toString().c_str() );
}
#include "libUtil/util.h"
//int test_time_now(int argc, char *argv[]);
int test_gpu_availability(int argc, char **argv);
int test_xEyes(int argc, char **argv);
//int test_appsrc_v2(int argc, char **argv);
int test_gst_rtsp_rcvH264_dec(int argc, char** argv);
int test_gst_rtsp_rcvH264_dec_and_save(int argc, char** argv);
//int test_bgSubtractor(int argc, char** argv);
int test_libCap(int argc, char** argv);

using namespace std;
using namespace xeyes;
int main(int argc, char* argv[])
{
	int x = 0;

#if 1
	x = test_xEyes(argc, argv);
#else
	//developing test approaches
	const string logFilename = "./log.txt";
	cout << logFilename <<endl;
	startLogThread(logFilename, true, true);
	dumpLog("---start---");

	//x = test_time_now(argc, argv);
	//x = test_gpu_availability(argc, argv);
	//x =test_appsrc_v2(argc, argv);
	//x = test_gpu_availability(argc, argv);

	//x = test_gst_rtsp_rcvH264_dec( argc, argv);
	x = test_gst_rtsp_rcvH264_dec_and_save( argc, argv);
	//x = test_bgSubtractor(argc, argv);
	//x = test_libCap( argc, argv);

	dumpLog("---endlog---");
	endLogThread();
#endif
	return x;
}


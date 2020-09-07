/*
*------------------------------------------------------------------------
*main.cpp
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
#include "libUtil/util.h"
//int test_time_now(int argc, char *argv[]);
int test_gpu_availability(int argc, char **argv);
int test_xEyes(int argc, char **argv);
//int test_appsrc_v2(int argc, char **argv);
int test_gst_rtsp_rcvH264_dec(int argc, char** argv);
int test_gst_rtsp_rcvH264_dec_and_save(int argc, char** argv);
//int test_bgSubtractor(int argc, char** argv);
int test_libCap(int argc, char** argv);
int test_detectnet_console( int argc, char** argv );

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
	//x = test_gst_rtsp_rcvH264_dec_and_save( argc, argv);
	//x = test_bgSubtractor(argc, argv);
	//x = test_libCap( argc, argv);

	x = test_detectnet_console( argc, argv);

	dumpLog("---endlog---");
	endLogThread();
#endif
	return x;
}


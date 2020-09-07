/*
*------------------------------------------------------------------------
*test_libCap.cpp
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
#include "libCfg/Cfg.h"
#include "libCap/CapAndSaveThread.h"
#include "libCap/CapSaveRtspH264.h"

using namespace std;
using namespace xeyes;

int test_CapAndSaveThread(int argc, char **argv);
int test_CapSaveRtspH264(int argc, char **argv);

int test_libCap(int argc, char** argv)
{
    //test_CapAndSaveThread( argc, argv);
    test_CapSaveRtspH264( argc, argv);
}


int test_CapSaveRtspH264(int argc, char **argv)
{
	CfgPtr cfg( new Cfg() );
	string cfgFile = "/home/swu/projects/xEyes/src/xEyes/res/cfg_xEyes.xml";
	printf("used cfg=<%s>\n", cfgFile.c_str());
	cfg->readFromFile(cfgFile.c_str());
    cout << cfg->toString() <<endl;

	printf("cfg read successfully\n" );

    DcUIPtr	dcUI(new DcUI(cfg));

    CapThreadBasePtr p0(new CapSaveRtspH264(0));
    p0->setCfg( cfg );
	p0->setDcUI( dcUI );
	p0->start();
	p0->wakeupToWork();
	while(1){
		THREAD_SLEEP(5000);
	}

	return 0;
}

int test_CapAndSaveThread(int argc, char** argv)
{
	CapAndSaveThread x;
	x.start();
	while(1){
		THREAD_SLEEP(5000);
	}
	return 0;
}




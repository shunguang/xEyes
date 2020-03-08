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




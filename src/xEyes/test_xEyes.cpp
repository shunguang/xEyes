#include "libUtil/util.h"
#include "libCfg/Cfg.h"

//#include "libDs/CudaUtil.h"
#include "libTest/TestCaps.h"
#include "libTest/TestGui.h"

using namespace std;
using namespace xeyes;
string locateCfgFile( int argc, char **argv);
void   startLog( const CfgPtr cfg );
void showUsage();

int test_xEyes(int argc, char **argv)
{
	//figure out config file
	string cfgFile = locateCfgFile( argc, argv );
	printf("used cfg=<%s>\n", cfgFile.c_str());

	//read cfg file
	CfgPtr cfg( new Cfg() );
	try{
		cfg->readFromFile(cfgFile.c_str());
	}
	catch (const std::overflow_error& e){
		printf("%s\n", e.what());	
	}
	printf("cfg read successfully\n" );

	//start log thread
	startLog( cfg );
	
	//cudaDeviceInit(0, NULL);

	//TestCaps x(cfg);
	//x.startThreads();

	QApplication app(argc, argv);
	QFont font;
	font.setFamily(font.defaultFamily());
	font.setPointSize(9);
	app.setFont(font);

	TestGui runGui(cfg);
	runGui.show();
	int ret = app.exec();

	endLogThread();
	return 0;
}

string locateCfgFile( int argc, char **argv)
{
	string cfgFile="";
	if (argc < 2) {
		string f0 = "cfg_xEyes.xml";
		string cfgFile1 = "./" + f0;            //for development
		if (boost::filesystem::exists(cfgFile1)) {
			cfgFile = cfgFile1;
		}
		else {								   //cfg at install folder
#if _WINDOWS
			cfgFile = "c:/Projects/xEyes/src/xEyes/res/" + f0;            //for development
#else
			cfgFile = "/home/swu/Projects/xEyes/src/xEyes/res/" + f0;            //for development
#endif
		}
	}
	else { //from cmd line
		cfgFile = string(argv[1]);
	}

	if (!boost::filesystem::exists(cfgFile)){
		printf("Error: Config file <%s> does not exit!", cfgFile.c_str());
		showUsage();
	}
	return cfgFile;
}

void   startLog( const CfgPtr cfg )
{
	//--------------------------------------------------------------------
	//create log file
	//--------------------------------------------------------------------
	CfgLog log = cfg->getLog();
	myCreateDir( log.logFolder );
	string ymd0 = ""; // getPrettyTimeStamp();
	string logFile = log.logFolder + "/logNgv" + ymd0 + ".txt";
	startLogThread(logFile, true, true);

	//dump current cfg into log file
	dumpLog("----------cfg----------------");
	string cfgStr = cfg->toString();
	std::vector<std::string> v;
	string_to_vector2(v, cfgStr, "\n", false);
	BOOST_FOREACH(const std::string &s, v){
		dumpLog(s);
	}
	//SetLoggerShowInConsoleFlag( log.ishowLogInConsole);
	//SetLoggerIsDumpFlag( log.isDumpLog);
	dumpLog("-------start main program ---------");
}
void showUsage()
{
	printf("usage: ./xeyes [cfg_file_path]\n");
	printf("Note:\n");
	printf("<./xeyes> will check config files by the following order:\n");
	printf("  [1] ~/Projects/xEyes/src/xEyes/res/cfg_xEyes.xml\n");
	printf("  [2] ./cfg_xEyes.xml (from deployed machine)\n");
}


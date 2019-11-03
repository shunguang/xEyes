#include "AppLog.h"
using namespace std;
using namespace xeyes;


//----------------- global funcs ---------------------
void xeyes::dumpLog( const std::string &x, ... )
{
	xeyes::dumpLog( x.c_str() );
}

void xeyes::dumpLog(const char *fmt, ...)
{
	//todo: remove this LOG_MAX_MSG_LEN, using dynamic allocation idea
	char buffer[LOG_MAX_MSG_LEN];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, LOG_MAX_MSG_LEN, fmt, args);
	va_end(args);

	//Logger::getInstance()->log(buffer);
	std::cout << buffer << std::endl;
}

//-------------------------------
void xeyes::startLogThread( const std::string &logFilename, const bool showInConsole, const bool dumpLog)
{
}

void xeyes::endLogThread()
{
}

void  xeyes::myExit(const int flag)
{
	if (flag != 0) {
		dumpLog("abnormal exit()!");
	}

	endLogThread();
	exit(1);
}

void  xeyes::myExit(const char * x, ...)
{
	dumpLog(x);
	endLogThread();
	exit(1);
}

void  xeyes::myExit(const  std::string &x, ...)
{
	dumpLog(x);
	endLogThread();
	exit(1);
}

void xeyes::myAssert( const bool flag, const std::string &msg)
{
	if (!flag) {
		dumpLog(msg);
		endLogThread();
		exit(1);
	}
}
void  xeyes::myAssert(const bool flag, const char *file, const int lineNum)
{
	if (!flag) {
		dumpLog( "Assert fail in File [%s], line[%d]", file, lineNum );
		endLogThread();
		exit(1);
	}
}


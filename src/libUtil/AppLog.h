#ifndef __APP_LOG_H__
#define __APP_LOG_H__

#include "DataTypes.h"
#include "UtilDefs.h"
namespace xeyes{
void  UTIL_EXPORT startLogThread(const std::string &logFilename, const bool showInConsole=true, const bool dumpLog=true );
void  UTIL_EXPORT endLogThread();

void  UTIL_EXPORT dumpLog(const char * x, ...);
void  UTIL_EXPORT dumpLog(const  std::string &x, ...);

void  UTIL_EXPORT myExit(const int flag);
void  UTIL_EXPORT myExit(const char * x, ...);
void  UTIL_EXPORT myExit(const  std::string &x, ...);
void  UTIL_EXPORT myAssert(const bool flag, const std::string &msg);
void  UTIL_EXPORT myAssert(const bool flag, const char *file, const int lineNum);
}
#endif


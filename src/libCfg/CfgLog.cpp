/*
*------------------------------------------------------------------------
*CfgLog.cpp
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
#include "CfgLog.h"

using namespace std;
using namespace xeyes;

CfgLog::CfgLog()
: CfgBase()
, logFolder("./")
, isDumpLog(true)
, ishowLogInConsole(false)
{
	for (int i = 0; i < THD_TASK_COUNT; ++i) {
		vFrmFreqToLog[i] = std::numeric_limits<int>::max();
	}
}


CfgLog::CfgLog( const CfgLog &x )
	: CfgBase( x )
	, logFolder(x.logFolder)
	, isDumpLog(x.isDumpLog)
	, ishowLogInConsole(x.ishowLogInConsole)
{
	for (int i = 0; i < THD_TASK_COUNT; ++i) {
		vFrmFreqToLog[i] = x.vFrmFreqToLog[i];
	}
}

CfgLog& CfgLog::operator = (const CfgLog &x)
{
	//todo: m-thread unsafe
	if (this != &x) {
		CfgBase::operator=(x);
		logFolder = x.logFolder;
		isDumpLog = x.isDumpLog;
		ishowLogInConsole = x.ishowLogInConsole;
		for (int i = 0; i < THD_TASK_COUNT; ++i) {
			vFrmFreqToLog[i] = x.vFrmFreqToLog[i];
		}
	}
	return *this;
}


void CfgLog::fromPropertyTree(const boost::property_tree::ptree &pt)
{
	appRootFolder = pt.get<std::string>("appRootFolder");
	logFolder = pt.get<std::string>("logFolder");
	isDumpLog = (bool)pt.get<int>("isDumpLog");
	ishowLogInConsole = (bool)pt.get<int>("isShowLogInConsole");

	vector<string> v;
	string s = pt.get<std::string>("frameFreqsToLog");
	int  n = string_to_vector2(v, s, ",", false);
	for (int i = 0; i < std::min<int>(n,THD_TASK_COUNT); ++i) {
		int m = atoi(v[i].c_str());
		if (m > 0) { vFrmFreqToLog[i] = m; }
	}
}

boost::property_tree::ptree CfgLog::toPropertyTree() 
{
	boost::property_tree::ptree pt;

	pt.put("appRootFolder", appRootFolder);
	pt.put("logFolder", logFolder);
	pt.put("isDumpLog", (int)isDumpLog);
	pt.put("ishowLogInConsole", (int)ishowLogInConsole);

	char buf[256];
	snprintf(buf, 256, "%d,%d,%d", vFrmFreqToLog[0], vFrmFreqToLog[1], vFrmFreqToLog[2]);
	pt.put("frameFreqsToLog", string(buf) );

	return pt;
}

std::string CfgLog::toString()
{
	boost::property_tree::ptree pt = toPropertyTree();
	return CfgBase::toString(pt);
}

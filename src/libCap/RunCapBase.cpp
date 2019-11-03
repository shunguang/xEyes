#include "RunCapBase.h"
#include "libDet/RunDetBase.h"
using namespace std;
using namespace xeyes;
RunCapBase::RunCapBase( const int camId, const int threadId, const std::string &threadName )
: ThreadX	( THD_TASK_CAP, threadId, threadName )
, m_camId 	( camId )
, m_camName	( "unkn" )
, m_frmNum(0)
, m_frmInterval_ms(40)
, m_yuvFrm_h(0)
, m_detPtr(0)
{
	cout << "RunDetBase::RunCapBase(): called" << endl;
}

RunCapBase::~RunCapBase()
{
}

void RunCapBase::setDetPtr( RunDetBase *p )
{
	m_detPtr = p;
}

void RunCapBase::dumpYuvFrm( const std::string &folderPath )
{
	m_yuvFrm_h->dump( folderPath, "capYuv" );
}

#include "CapThreadBase.h"
#include "libDet/DetThreadBase.h"
using namespace std;
using namespace xeyes;
CapThreadBase::CapThreadBase( const int camId, const int threadId, const std::string &threadName )
: ThreadX	( THD_TASK_CAP, threadId, threadName )
, m_camId 	( camId )
, m_frmNum(0)
, m_frmInterval_ms(40)
, m_yuvFrm_h(0)
, m_detPtr(0)
{
	cout << "DetThreadBase::CapThreadBase(): called" << endl;
}

CapThreadBase::~CapThreadBase()
{
}

void CapThreadBase::setDetPtr( DetThreadBase *p )
{
	m_detPtr = p;
}

void CapThreadBase::dumpYuvFrm( const std::string &folderPath )
{
	m_yuvFrm_h->dump( folderPath, "capYuv" );
}

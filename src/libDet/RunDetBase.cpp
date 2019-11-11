#include "libDsp/RunDsp.h"
#include "RunDetBase.h"
using namespace std;
using namespace xeyes;
RunDetBase::RunDetBase( const int camId, const int threadId, const std::string &threadName )
: ThreadX	( THD_TASK_DET, threadId, threadName )
, m_camId 	( camId )
, m_frmNum(0)
, m_yuvFrm_h(0)
, m_detFrm_h(0)
, m_camDc(0)
, m_dspPtr(0)
{
	cout << "RunDetBase::RunDetBase(): called" << endl;
}

RunDetBase::~RunDetBase()
{
}

void RunDetBase::setDspPtr(RunDsp *p)
{
	m_dspPtr = p;
}

void RunDetBase::dumpFrm( const std::string &folderPath )
{
	m_detFrm_h->dump( folderPath );
}

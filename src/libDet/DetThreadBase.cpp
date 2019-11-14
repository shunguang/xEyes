#include "libDsp/DspThread.h"
#include "DetThreadBase.h"
using namespace std;
using namespace xeyes;
DetThreadBase::DetThreadBase( const int camId, const int threadId, const std::string &threadName )
: ThreadX	( THD_TASK_DET, threadId, threadName )
, m_camId 	( camId )
, m_frmNum(0)
, m_yuvFrm_h(0)
, m_detFrm_h(0)
, m_camDc(0)
, m_dspPtr(0)
{
	cout << "DetThreadBase::DetThreadBase(): called" << endl;
}

DetThreadBase::~DetThreadBase()
{
}

void DetThreadBase::setDspPtr(DspThread *p)
{
	m_dspPtr = p;
}

void DetThreadBase::dumpFrm( const std::string &folderPath )
{
	m_detFrm_h->dump( folderPath );
}

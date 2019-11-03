#include "RunDetBase.h"

using namespace std;
using namespace xeyes;
RunDetBase::RunDetBase( const int camId, const int threadId, const std::string &threadName )
: ThreadX	( THD_TASK_DET, threadId, threadName )
, m_camId 	( camId )
, m_camName	( "unkn" )
, m_frmNum(0)
, m_yuvFrm_h(0)
, m_rgbFrm_h(0)
{
	cout << "RunDetBase::RunDetBase(): called" << endl;
}

RunDetBase::~RunDetBase()
{
}

void RunDetBase::dumpRgbFrm( const std::string &folderPath )
{
	m_rgbFrm_h->dump( folderPath, "detRgb" );
}

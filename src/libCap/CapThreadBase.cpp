/*
*------------------------------------------------------------------------
*CapThreadBase.cpp
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
, m_detPtr(NULL)
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

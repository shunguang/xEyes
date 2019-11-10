#include "DetFrm_h.h"
using namespace std;
using namespace xeyes;


DetFrm_h::DetFrm_h( const int L, const uint64_t fn)
: m_L (L)            //detetcion pyramid level 
, m_fn (fn)			//frm # in file
, m_vRois()
{
}

void DetFrm_h::dump(const std::string &folderPath) const
{
}

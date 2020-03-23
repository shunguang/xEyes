#include "DetFrm_h.h"
using namespace std;
using namespace xeyes;


DetFrm_h::DetFrm_h( const int w0, const int h0, const int L, const uint64_t fn)
: m_L (L)            //detetcion pyramid level 
, m_fn (fn)			//frm # in file
, m_vRois()
, m_rgbImg(w0>>L, h0>>L, L, fn)
{
}

DetFrm_h::DetFrm_h( const DetFrm_h &x)
: m_L ( x.m_L )
, m_fn( x.m_fn )
, m_vRois( x.m_vRois )
{
    m_rgbImg.hdCopyFrom( &x.m_rgbImg );
}

DetFrm_h& DetFrm_h:: operator = (const DetFrm_h &x)
{
     hdCopyFrom( &x );
}

void DetFrm_h::dump(const std::string &folderPath, const std::string &tag) const
{
	m_rgbImg.dump(folderPath, tag);
}

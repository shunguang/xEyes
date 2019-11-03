#include "RgbFrm_h.h"
using namespace std;
using namespace cv;
using namespace xeyes;

RgbFrm_h :: RgbFrm_h( int w, int h, int L, uint64_t fn )
: L_(L)
, I_(w,h,CV_8UC3)
, fn_(fn)
{
}

void RgbFrm_h::hdCopyFrom( const RgbFrm_h *src )
{
    L_ = src->L_;
    fn_ = src->fn_;
    src->I_.copyTo( I_ );
}

void RgbFrm_h::hdCopyTo( RgbFrm_h *dst ) const
{
    dst->L_  =   L_;
    dst->fn_ =   fn_;
    I_.copyTo( dst->I_ );
}

void RgbFrm_h::dump(const std::string &dumpFolder, const std::string &prefix)
{
}

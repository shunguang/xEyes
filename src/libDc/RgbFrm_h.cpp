/*
*------------------------------------------------------------------------
*RgbFrm_h.cpp
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
#include "RgbFrm_h.h"
using namespace std;
using namespace cv;
using namespace xeyes;

RgbFrm_h :: RgbFrm_h( int w, int h, int L, uint64_t fn )
: L_(L)
, I_(h,w,CV_8UC3)
, fn_(fn)
{
}

void RgbFrm_h::resize(int w, int h, int L)
{
    L_=L;
    I_.create(h,w,CV_8UC3);
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

void RgbFrm_h::hdCopyFromBuf(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn)
{
    uint32_t sz = I_.cols * I_.rows * 3;
	myAssert( bufSz == sz, "RgbFrm_h::hdCopyFromBuf(): buf sz doest match, data loss!");
	memcpy(I_.data, buf, bufSz);
	fn_ = fn;    
}

void RgbFrm_h::dump(const std::string &dumpFolder, const std::string &prefix) const
{
	char fpath[1024];
	snprintf(fpath, 1024, "%s/%s-fn-%09llu_L%d.png", dumpFolder.c_str(), prefix.c_str(), fn_, L_);
	cv::imwrite(fpath, I_);
}

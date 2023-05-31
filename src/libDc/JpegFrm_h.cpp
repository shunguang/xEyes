/*
*------------------------------------------------------------------------
*JpegFrm_h.cpp
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
#include "JpegFrm_h.h"
using namespace std;
using namespace xeyes;

JpegFrm_h::JpegFrm_h(const uint32_t  sz, const uint64_t fn)
	: fn_( fn)
	, sz_( sz )
	, v_( sz )
{
}


JpegFrm_h::JpegFrm_h(const JpegFrm_h &x) 
	: fn_(x.fn_)
	, sz_(x.sz_)
	, v_(x.v_)
{
}

JpegFrm_h &JpegFrm_h::operator = (const JpegFrm_h &x) 
{
	if (&x != this) { //check for self assignment
		fn_ = x.fn_;
		sz_ = x.sz_;
		v_  = x.v_;
	}
	return *this; // enables x=y=z;
}

void JpegFrm_h::hdCpyFrom(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn) 
{
	if ( bufSz > sz_) {
		myAssert( false, "JpegFrm_h::copyFrom(): <bufSz> is too large, quit!");
		return;
	}

	memcpy(v_.data(), buf, bufSz);
	fn_ = fn;
	sz_ = bufSz;
}

void JpegFrm_h::dump(const std::string &folder, const std::string &filePrefixTag)
{
	dumpJpg(folder, filePrefixTag, fn_, v_.data(), sz_);

}

//the floowingt are static functions
std::string  JpegFrm_h::getJpgFileName(const std::string &folder, const std::string &filePrefixTag, const uint64_t fn)
{
	char ff[256];
	snprintf(ff, 256, "%s/%sI-%06d.jpg", folder.c_str(), filePrefixTag.c_str(), fn);
	return std::string(ff);
}

uint64_t JpegFrm_h::getJpgImgNum(const std::string &nameWoFolder, const std::string &filePrefixTag)
{
	uint32_t L = filePrefixTag.length();
	uint64_t fn = std::stoi( nameWoFolder.substr(L + 2, 6) );
	return fn;
}

void JpegFrm_h::dumpJpg(const std::string &folder, const std::string &filePrefixTag, const uint64_t fn, const uint8_t *buf,  const uint32_t sz)
{
	std::string ff = getJpgFileName(folder, filePrefixTag, fn);
	std::ofstream outfile(ff, ios::binary | ios::out);
	if (outfile.is_open()) {
		outfile.write((const char *)buf, sz);
		outfile.close();
		return;
	}
	dumpLog("JpegFrm_h::dumpJpg(): cannot open file=[%s], sz=%d", ff.c_str(), sz);
}

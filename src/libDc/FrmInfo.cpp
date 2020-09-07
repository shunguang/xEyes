/*
*------------------------------------------------------------------------
*FrmInfo.cpp
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
#include "FrmInfo.h"
using namespace std;
using namespace xeyes;
FrmInfo::FrmInfo(int w0, int h0, int detPyrL, int dspPyrL, uint64_t fn)
	: fn_(fn)
	, yuv_(w0, h0, fn)
	, det_(w0, h0, detPyrL, fn)
	, dsp_(w0 >> dspPyrL, h0 >> dspPyrL)
{
}

FrmInfo::FrmInfo(const FrmInfo &x)
	: fn_	( x.fn_)
	, yuv_	( x.yuv_ )
	, det_	( x.det_)
	, dsp_	( x.dsp_ )
{
}

void FrmInfo::resetSize(int w0, int h0, int detPyrL, int dspPyrL)
{
	yuv_.resetSz(w0, h0);
	det_.resetSz(w0,h0,detPyrL);
	dsp_.resetSz(w0 >> dspPyrL, h0 >> dspPyrL);
}


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


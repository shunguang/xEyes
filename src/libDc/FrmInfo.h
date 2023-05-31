/*
*------------------------------------------------------------------------
*FrmInfo.h
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
//this thread capture Bayer images from camera and store them into a circluar que
#ifndef __FRM_INFO_H__
#define __FRM_INFO_H__

#include "libUtil/util.h"
#include "YuvFrm_h.h"

#if APP_USE_CUDA
#include "YuvFrm_d.h"
#endif

#include "RgbFrm_h.h"
#include "DetFrm_h.h"
#include "DspFrm_h.h"

namespace xeyes {
	class FrmInfo {
	public:
		FrmInfo( int w0=0, int h0=0, int detPyrL=0, int dspPyrL=0, uint64_t fn=0 );
		FrmInfo( const FrmInfo &x );
		~FrmInfo() = default;
	
		void resetSize(int w0, int h0, int detPyrL, int dspPyrL );

	public:
		//todo: remove <fn_> inside YuvFrm_h, detFrm_h, and DspFrm_h
		uint64_t	fn_;	 //frame number
		YuvFrm_h	yuv_;	 //always at L0	
		DetFrm_h	det_;	 //at detettcion Level
		DspFrm_h	dsp_;	 //at display Level

	};
	typedef std::shared_ptr<FrmInfo> FrmInfoPtr;
}
#endif

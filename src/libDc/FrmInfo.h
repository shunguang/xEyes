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

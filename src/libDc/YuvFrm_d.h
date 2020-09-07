/*
*------------------------------------------------------------------------
*YuvFrm_d.h
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
#ifndef __YUV420Frm_D_H__
#define __YUV420Frm_D_H__
#include "libUtil/util.h"

#if APP_USE_CUDA
#include <cuda_runtime.h>
#include <npp.h>
#include <nppi.h>
#include "YuvFrm_h.h"
#include "RgbFrm_d.h"

namespace xeyes {
	class YuvFrm_d {
	public:
		YuvFrm_d(int w=0, int h=0);
		~YuvFrm_d();

		void create(int w, int h);
		void resize(int w, int h);

		//note: we keep poiters as unified interface which can handle smartpointers, regular YUV or RGB Frms efficently
		void hdCopyFrom(const YuvFrm_d *src); 	//frem YUV dev
		void hdCopyFrom(const YuvFrm_h *src); 	//from YUV host
		void hdCopyFromRgb_d(const RgbFrm_d *src);	//from RGB dev
		void hdCopyFromHostBuf(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn = 0);

		//hd copy to for same szie image
		void hdCopyTo(YuvFrm_d *dst) const;   //copy to device
		void hdCopyTo(YuvFrm_h *dst) const;   //copy host same size
		void hdCopyTo2(YuvFrm_h *dst) const;  //copy to for dst size > src size becasue RTSP need special image sizes

		void dump(const std::string &folder, const std::string &tag = "", int roiW = 0, int roiH = 0, int L = 0);
		void print(const std::string &msg);
	public:
		Npp8u*		pImg_;
		uint64_t	fn_;
		uint32_t	ip_;        //this img goes to which client
		NppiSize	size_;      //size to allocate memeory
		NppiRect    roi_;       //ROI meanningful, (we may allocate more data when init)

		Npp8u*		pBuf_[3];
		int			nSteps_[3];
		uint32_t	nBufSz_[3];
	};
	typedef std::shared_ptr<YuvFrm_d> YuvFrm_dPtr;
}
#endif
#endif

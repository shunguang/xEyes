/*
*------------------------------------------------------------------------
*RgbFrm_d.h
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
#ifndef __RGB_FRM_D_H__
#define __RGB_FRM_D_H__

#include "libUtil/util.h"
#if APP_USE_CUDA
#include <cuda_runtime.h>
#include <npp.h>
#include <nppi.h>
namespace xeyes {
	class RgbFrm_d {
	public:
		RgbFrm_d(int w=0, int h=0);
		~RgbFrm_d();
		void freeImg();
		void create(int w, int h);
		void setValue(int val);

		//note: make sure the channel order of <src> is RGB, cv::Mat stores RGB img in the order of BGR!
		//otherwise you need to cvt the color order by:	cv::cvtColor(I1, I2, CV_BGR2RGB);
		void hdCopyFrom_host(const cv::Mat *src);
		bool hdCopyFrom_dev( const RgbFrm_d &src );
		void hdCopyTo_host(cv::Mat &rgb_host) const;
		void hdCopyRoiTo_host(cv::Mat &rgb_host) const;
		void hdCopyTo_dev(RgbFrm_d *dst ) const;
		void hdCopyRoiTo_dev(RgbFrm_d *dst) const;
		void resizeRoiTo_dev(RgbFrm_d *dst) const;
		void setRoi(int x, int y, int w, int h);
		void dump(const std::string &tag, int L, const std::string &dumpFolder = ".");
		void print(const std::string &msg) {
			printf("%s, nStep_=%d, size_(w=%d,h=%d)\n", msg.c_str(), nStep_, size_.width, size_.height);
		}
	public:
		Npp8u*		pImg_;
		int			nStep_;
		NppiSize	size_;
		uint64_t	fn_;
		NppiRect    roi_;
	};
	typedef std::shared_ptr<RgbFrm_d> RgbFrm_dPtr;
}
#endif
#endif

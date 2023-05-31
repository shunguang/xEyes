/*
*------------------------------------------------------------------------
*RgbFrm_h.h
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
#ifndef __RGB_FRM_H_H__
#define __RGB_FRM_H_H__

#include "libUtil/util.h"
namespace xeyes {
	class RgbFrm_h {
	public:
		RgbFrm_h( int w=0, int h=0, int L=0, uint64_t fn=0 );
		RgbFrm_h( const RgbFrm_h &x ) = default;
		~RgbFrm_h() = default;
		
		void hdCopyFrom( const RgbFrm_h *src );
		void hdCopyTo( RgbFrm_h *dst ) const;
		void hdCopyFromBuf(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn = 0);

		void resize(int w, int h, int L);
		//dump img into a file name start with <prefix>
		void dump(const std::string &dumpFolder = ".", const std::string &prefix="rgbFrm" ) const;
		void print(const std::string &msg) {
			printf("%s, size_(w=%d,h=%d), L_=%d\n", msg.c_str(), I_.cols, I_.rows, L_);
		}

	public:
        int         L_;    //pyramid level of the original image which comes from
		cv::Mat		I_;    //cv img at level <L_>
		uint64_t	fn_;   //frame number
	};
	typedef std::shared_ptr<RgbFrm_h> RgbFrm_hPtr;
}
#endif

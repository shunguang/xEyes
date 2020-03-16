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

		//dump img into a file name start with <prefix>
		void dump(const std::string &dumpFolder = ".", const std::string &prefix="rgbFrm" );
		void print(const std::string &msg) {
			printf("%s, size_(w=%d,h=%d), L_=%d\n", msg.c_str(), I_.cols, I_.rows, L_);
		}

	public:
        int         L_;    //pyramid level
		cv::Mat		I_;    //cv img at level <L_>
		uint64_t	fn_;   //frame number
	};
	typedef std::shared_ptr<RgbFrm_h> RgbFrm_hPtr;
}
#endif

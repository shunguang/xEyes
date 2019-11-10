#ifndef __YUV420_FRM_H_H__
#define __YUV420_FRM_H_H__

#include "libUtil/util.h"
#include "DsDefs.h"
namespace xeyes {
	class DS_EXPORT YuvFrm_h {
	public:
		YuvFrm_h(const int w=0, const int h=0, const uint64_t fn=0 );  //sz = w*h*3/2
		YuvFrm_h(const cv::Size &imgSz, const uint64_t fn);
		YuvFrm_h(const cv::Mat &bgr, const uint64_t fn=0);
		//soft copy, assert(bufSz == w*h*3/2)
		//YuvFrm_h(const int w, const int h, uint8_t *buf_, uint32_t bufSz, const uint64_t fn );

		//copy construct
		YuvFrm_h(const YuvFrm_h &x);
		~YuvFrm_h()=default;

		YuvFrm_h& operator = (const YuvFrm_h &x);
		void resetSz(const int w, const int h );

		void hdCopyTo(YuvFrm_h *des) const;			//same size copy
		void hdCopyToLargerDst(YuvFrm_h *dst) const;	//dst.sz_ > src.sz_ copy
		void hdCopyToBGR(cv::Mat *dstBGR) const;

		void hdCopyFrom( const YuvFrm_h *src );
		void hdCopyFromBuf(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn = 0);
		void hdCopyFromBGR(const cv::Mat *srcBGR, const uint64_t fn=0);

		void setToZeros() {
			v_.assign(sz_,0);
		}

		void dump(const std::string &folder, const std::string &tag = "", int roiW = 0, int roiH = 0, int L = 0);
	private:
		void setFeatures();

	public:
		uint64_t  	fn_;				//frm # in original video for debug purpose
		int			w_;					//width and height if its a video frm
		int     	h_;
		uint32_t  	sz_;				//buf size, assert( sz_ == w_*h_*3/2 );
		bool		isKeyFrm_;
		std::vector<uint8_t> v_;		//<v_> hold the YUV420 image data
										//where YUV420p is a planar format, meaning that the Y, U, and V values are grouped together instead of interspersed.
										//The reason for this is that by grouping the U and V values together, the image becomes much more compressible.
										//When given an array of an image in the YUV420p format, all the Y? values come first, followed by all the U values, 
										//followed finally by all the V values.
										//to see the image: call yuv420p_save(const uint8_t *yuv420pRaw, const int sz, const int w, const int h, const char *fname);
		uint8_t		*pBuf_[3];        	//pBuf_[0] = buf_; pBuf_[1] = buf_ + w_*h_; pBuf_[2] = buf_ + w_*h_ * 5 / 4;
		uint32_t	nBufSz_[3];       	//nBufSz_[0] = w_*h_; nBufSz_[1] = w_*h_/4; nBufSz_[2] = w_*h_/4

	};
	typedef std::shared_ptr<YuvFrm_h>		YuvFrm_hPtr;
}
#endif


//this thread capture Bayer images from camera and store them into a circluar que
#include "YuvFrm_d.h"
#if APP_USE_CUDA

using namespace std;
using namespace xeyes;
YuvFrm_d::YuvFrm_d(int w, int h)
	: pImg_(0)
	, fn_(0)
	, ip_(0)
	, size_{0,0}
	, roi_{0,0,0,0}
{
	create(w,h);
}

YuvFrm_d::~YuvFrm_d()
{
	if (pImg_) {
		cudaFree(pImg_);
	}
}

void YuvFrm_d::create(int w, int h)
{
	if (w*h>0){
		cudaMalloc((void **)&pImg_, w*h * 3 / 2);
		pBuf_[0] = pImg_;
		pBuf_[1] = pImg_ + w*h;
		pBuf_[2] = pImg_ + w*h * 5 / 4;
	}
	else{
		pBuf_[0] = 0;
		pBuf_[1] = 0;
		pBuf_[2] = 0;
	}
	size_ = {w, h};
	roi_= {0,0,w,h};

	nSteps_[0] = w; nSteps_[1] = w / 2; nSteps_[2] = w / 2;
	nBufSz_[0] = w*h; nBufSz_[1] = w*h / 4; nBufSz_[2] = w*h / 4;
}

void YuvFrm_d::resize(int w, int h)
{
	if (pImg_) {
		cudaFree(pImg_);
	}
	create(w, h);
}

//copy from dev
void YuvFrm_d::hdCopyFrom(const YuvFrm_d *src)
{
	int w = src->size_.width;
	int h = src->size_.height;
	if (w != size_.width || h != size_.height) {
		myExit("hdCopyFrom(): size_ does not match, dst[w=%d,h=%d], src[w=%d,h=%d]", size_.width, size_.height, w, h);
	}
	fn_ = src->fn_;
	int n = w*h * 3 / 2;
	cudaMemcpy(pImg_, src->pImg_, n, cudaMemcpyDeviceToDevice);
}

//copy from host
void YuvFrm_d::hdCopyFrom(const YuvFrm_h *src)
{
	if (src->w_ != size_.width || src->h_ != size_.height) {
		myExit("hdCopyFrom(): size_ does not match, dst[w=%d,h=%d], src[w=%d,h=%d]", size_.width, size_.height, src->w_, src->h_);
	}
	fn_ = src->fn_;
	cudaMemcpy(pImg_, src->v_.data(), src->sz_, cudaMemcpyHostToDevice);
}

void YuvFrm_d::hdCopyFromRgb_d(const RgbFrm_d *src) {
	//assert(I.size_.width >= size_.width &&  I.size_.height >= size_.height);
	if (src->size_.width < size_.width || src->size_.height < size_.height ) {
		myExit("hdCopyFromRgb_d(): size_ does not match, RGB[w=%d,h=%d], YUV[w=%d,h=%d]", src->size_.width, src->size_.height, size_.width, size_.height);
	}

	int ww = APP_MIN(src->roi_.width, size_.width);
	int hh = APP_MIN(src->roi_.height, size_.height);

	NppiSize szToCpy = { ww, hh };
	//NppiSize roiToCpy = {I.size_.width, I.size_.height};
	nppiRGBToYCbCr420_8u_C3P3R(src->pImg_, src->nStep_, pBuf_, nSteps_, szToCpy);

	roi_ = {src->roi_.x, src->roi_.y, src->roi_.width, src->roi_.height};
	fn_  = src->fn_;
}

void YuvFrm_d::hdCopyFromHostBuf(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn)
{
	myAssert( bufSz == 3*(size_.width * size_.height)/2, "YuvFrm_d::hdCopyFromHostBuf(): buf sz doest match, data loss!");
	cudaMemcpy(pImg_, buf, bufSz, cudaMemcpyHostToDevice);
	fn_ = fn;
}

void YuvFrm_d::hdCopyTo(YuvFrm_d *dst) const
{
	const int w = dst->size_.width;
	const int h = dst->size_.height;
	if (w != size_.width || h != size_.height) {
		//myExit("YuvFrm_d::hdCopyTo_dev() size_ does not match, dst[w=%d,h=%d], src[w=%d,h=%d]", w, h, size_.width, size_.height);
		dumpLog("YuvFrm_d::hdCopyTo_dev(): warning -- size_ does not match, dst[w=%d,h=%d], src[w=%d,h=%d]", w, h, size_.width, size_.height);
		return;
	}
	dst->fn_ = fn_;
	int n = w*h*3 / 2;
	cudaMemcpy(dst->pImg_, pImg_, n, cudaMemcpyDeviceToDevice);
}

//hd copy to for same size_ image at host memoery
void YuvFrm_d::hdCopyTo(YuvFrm_h *dst) const
{
	if (dst->w_ != size_.width || dst->h_ != size_.height) {
		//myExit("YuvFrm_d::hdCopyTo_host(): size_ does not match, dst[w=%d,h=%d], src[w=%d,h=%d]", dst->w_, dst->h_, size_.width, size_.height);
		dumpLog("YuvFrm_d::hdCopyTo(): warning --size_ does not match, dst[w=%d,h=%d], src[w=%d,h=%d]", dst->w_, dst->h_, size_.width, size_.height);
		return;
	}

	dst->fn_ = fn_;
	cudaMemcpy(dst->v_.data(), pImg_, dst->sz_, cudaMemcpyDeviceToHost);
}

//hd copy to <dst>: for dst size_ > src size_ becasue RTSP need special image sizes
void YuvFrm_d::hdCopyTo2(YuvFrm_h *dst) const {
	if (dst->w_ < size_.width || dst->h_ < size_.height) {
		myExit("hdCopyTo2(): size_ does not match, dst[w=%d,h=%d], src[w=%d,h=%d]", dst->w_, dst->h_, size_.width, size_.height);
	}
	dst->fn_ = fn_;

	//copy Y
	int wSrc = size_.width, hSrc = size_.height;
	int wDst = dst->w_;
	for (int ch = 0; ch < 3; ++ch) {
		if (ch > 0) {
			wSrc = size_.width / 2;
			hSrc = size_.height / 2;
			wDst = dst->w_ / 2;
		}

		uint8_t *pDst = dst->pBuf_[ch];
		uint8_t *pSrc = pBuf_[ch];
		for (int y = 0; y < hSrc; ++y, pDst += wDst, pSrc += wSrc) {
			cudaMemcpy(pDst, pSrc, wSrc, cudaMemcpyDeviceToHost);
		}
	}
}


void YuvFrm_d::dump(const std::string &folder, const std::string &tag, int roiW, int roiH, int L)
{
	YuvFrm_h f(size_.width, size_.height, fn_);
	cudaMemcpy(f.v_.data(), pImg_, f.sz_, cudaMemcpyDeviceToHost);
	f.dump( folder, tag, roi_.width, roi_.height, L);
}

void YuvFrm_d::print(const std::string &msg)
{
	printf("%s, nSteps_=[%d,%d,%d], size_(w=%d,h=%d)\n", msg.c_str(), nSteps_[0], nSteps_[1], nSteps_[2], size_.width, size_.height);
}
#endif

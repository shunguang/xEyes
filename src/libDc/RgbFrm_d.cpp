/*
*------------------------------------------------------------------------
*RgbFrm_d.cpp
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
#include "RgbFrm_d.h"
#if APP_USE_CUDA

using namespace std;
using namespace xeyes;

RgbFrm_d::RgbFrm_d(int w, int h) 
: pImg_(0)
, nStep_(0)
, size_{0,0}
, fn_(0)
, roi_{0,0,0,0}
{
	create(w, h);
}

RgbFrm_d::~RgbFrm_d() {
	freeImg();
}

void RgbFrm_d::freeImg() {
	if (pImg_ != 0) {
		cudaFree(pImg_);
		pImg_ = 0;
		size_ = { 0, 0 };
		roi_  = { 0,0,0,0 };
	}
}

void RgbFrm_d::create(int w, int h) {
	//2D 3chs: [RGBRGB...RGB] -- 3*w
	if (w == size_.width && h == size_.height ) {
		return;
	}

	freeImg();
	if(w*h>0 ){
		pImg_ = nppiMalloc_8u_C3(w, h, &nStep_);
		size_.width = w;  size_.height = h;
		roi_.x = 0; roi_.y = 0; roi_.width = w; roi_.height = h;
	}
	else{
		pImg_ = 0;
		size_ = {0,0};
		roi_  = {0,0,0,0};
	}
}

void RgbFrm_d::setValue(int val) {
	cudaMemset2D((void *)pImg_, 3 * size_.width, val, size_.width, size_.height);
}

//note: make sure the channel order of <src> is RGB, cv::Mat stores RGB img in the order of BGR!
//otherwise you need to cvt the color order by:	cv::cvtColor(I1, I2, CV_BGR2RGB);
void RgbFrm_d::hdCopyFrom_host(const cv::Mat *src) {
	myAssert(src->cols <= size_.width && src->rows <= size_.height, "RgbFrm_d::hdCopyFrom_host(): size_ desen't macth!");
	cudaMemcpy2D((void *)pImg_, nStep_, (void *)src->data, src->step, 3 * src->cols, src->rows, cudaMemcpyHostToDevice);
}

bool RgbFrm_d::hdCopyFrom_dev( const RgbFrm_d &src ) {
	//dumpLog("hdCopyFrom_dev(): did not copy! -- src size_(%d,%d), dst size_(%d,%d)", src.size_.width, src.size_.height, size_.width, size_.height);
	//myAssert(src.size_.width == size_.width && src.size_.height == size_.height, "RgbFrm_d::hdCopyFrom_dev(): size_ desen't macth!");
	int w = APP_MIN(size_.width, src.size_.width);
	int h = APP_MIN(size_.height, src.size_.height);
	cudaMemcpy2D((void *)pImg_, nStep_, (void *)src.pImg_, src.nStep_, 3 * w, h, cudaMemcpyDeviceToDevice);
	fn_ = src.fn_;
	return true;
}

void RgbFrm_d::hdCopyTo_host(cv::Mat &rgb_host) const {
	myAssert(rgb_host.cols == size_.width && rgb_host.rows == size_.height, "RgbFrm_d::hdCopyTo_host(): size_ desen't macth!");
	cudaMemcpy2D((void *)rgb_host.data, rgb_host.step, (void *)pImg_, nStep_, 3 * size_.width, size_.height, cudaMemcpyDeviceToHost);
}

void RgbFrm_d::hdCopyRoiTo_host(cv::Mat &rgb_host) const {
	if (rgb_host.cols != roi_.width || rgb_host.rows != roi_.height) {
		rgb_host.create(roi_.height, roi_.width, CV_8UC3);
	}
	Npp8u * p_src = pImg_ + roi_.y * nStep_ + roi_.x * 3 * sizeof(Npp8u);
	cudaMemcpy2D((void *)rgb_host.data, rgb_host.step, (void *)p_src, nStep_, 3 * roi_.width, roi_.height, cudaMemcpyDeviceToHost);
}

void RgbFrm_d::hdCopyTo_dev(RgbFrm_d *dst ) const {
	int w = APP_MIN(size_.width, dst->size_.width);
	int h = APP_MIN(size_.height, dst->size_.height);
	//myAssert(dst->size_.width == size_.width && dst->size_.height == size_.height, "RgbFrm_d::hdCopyTo_dev(): size_ desen't macth!");
	cudaMemcpy2D((void *)dst->pImg_, dst->nStep_, (void *)pImg_, nStep_, 3*w, h, cudaMemcpyDeviceToDevice);
	dst->fn_ = fn_;
}

void RgbFrm_d::hdCopyRoiTo_dev(RgbFrm_d *dst) const {
	myAssert(dst->size_.width >= roi_.width && dst->size_.height >= roi_.height, "RgbFrm_d::hdCopyRoiTo_dev(): size_ desen't macth!");

	if ( dst->size_.width > roi_.width || dst->size_.height > roi_.height ) {
		cudaMemset2D((void *)dst->pImg_, dst->nStep_, 0, 3*dst->size_.width, dst->size_.height);
	}
	int y0 = (dst->size_.height - roi_.height) / 2;
	int x0 = (dst->size_.width  - roi_.width) / 2;
	Npp8u * p_des = dst->pImg_ + y0 * dst->nStep_ + 3 * x0 * sizeof(Npp8u);
	Npp8u * p_src = pImg_ + roi_.y * nStep_   + 3 * roi_.x * sizeof(Npp8u);
	cudaMemcpy2D((void *)p_des, dst->nStep_, (void *)p_src, nStep_, 3 * roi_.width, roi_.height, cudaMemcpyDeviceToDevice);
	dst->fn_ = fn_;
}

void RgbFrm_d::resizeRoiTo_dev(RgbFrm_d *dst) const {
	const Npp8u * pSrc = pImg_ + roi_.y * nStep_ + roi_.x * 3 * sizeof(Npp8u);
	NppiSize oSrcSize = { roi_.width, roi_.height };
	NppiRect oDstRectROI = { 0, 0, dst->size_.width, dst->size_.height };
	int eInterploationMode = NPPI_INTER_SUPER;
	if ((float)dst->size_.width / (float)roi_.width >= 1.f) {
		eInterploationMode = NPPI_INTER_LANCZOS;
	}

	nppiResize_8u_C3R( pSrc, nStep_, oSrcSize, roi_, dst->pImg_, dst->nStep_, dst->size_, oDstRectROI, eInterploationMode);
	dst->fn_ = fn_;
}

void RgbFrm_d::setRoi(int x, int y, int w, int h) {
	roi_.x = x; roi_.y = y; roi_.width = w; roi_.height = h;
}

void RgbFrm_d::dump(const std::string &tag, int L, const std::string &dumpFolder) {
	cv::Mat I(roi_.height, roi_.width, CV_8UC3);   //RGB
	Npp8u * p_src = pImg_ + roi_.y * nStep_ + roi_.x * 3 * sizeof(Npp8u);
	cudaMemcpy2D((void *)I.data, 3 * roi_.width, (void *)p_src, nStep_, 3 * roi_.width, roi_.height, cudaMemcpyDeviceToHost);
	
	cv::Mat I2;
	cv::cvtColor(I, I2, CV_RGB2BGR);
	myImgWrite(I2, dumpFolder, tag, fn_, L);
}
#endif

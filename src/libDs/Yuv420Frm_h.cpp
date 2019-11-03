#include "Yuv420Frm_h.h"
using namespace std;
using namespace xeyes;

Yuv420Frm_h::Yuv420Frm_h( const int w, const int h, const uint64_t fn )
	: fn_(fn)
	, w_(w)
	, h_(h)
	, sz_( 3*(w*h/2) )
	, isKeyFrm_(false)
	, v_(sz_)
{
	setFeatures();
}

Yuv420Frm_h::Yuv420Frm_h(const cv::Size &imgSz, const uint64_t fn)
	: fn_(fn)
	, w_(imgSz.width)
	, h_(imgSz.height)
	, sz_((imgSz.width*imgSz.height * 3) / 2)
	, isKeyFrm_(false)
	, v_(sz_)
{
	setFeatures();
}

Yuv420Frm_h::Yuv420Frm_h(const cv::Mat &bgr, const uint64_t fn)
	: fn_(fn)
	, w_(bgr.cols)
	, h_(bgr.rows)
	, sz_( (w_*h_) / 2 )
	, isKeyFrm_(false)
	, v_( sz_ )
{
	setFeatures();
	hdCopyFromBGR(&bgr, fn);
}

//soft copy 
#if 0
Yuv420Frm_h::Yuv420Frm_h( const int w, const int h, uint8_t *buf, uint32_t bufSz, const uint64_t fn )
	: fn_(fn)
	, w_(w)
	, h_(h)
	, sz_(bufSz)
	, isKeyFrm_(false)
{
	assert( bufSz == (w*h*3)/2);
	setFeatures();
}
#endif

Yuv420Frm_h::Yuv420Frm_h(const Yuv420Frm_h &x) 
	: fn_(x.fn_)
	, w_ (x.w_)
	, h_ (x.h_)
	, sz_(x.sz_)
	, isKeyFrm_(x.isKeyFrm_)
	, v_(x.v_)
{
	setFeatures();
}

Yuv420Frm_h &Yuv420Frm_h::operator = (const Yuv420Frm_h &x) 
{
	if (&x != this) { //check for self assignment
		fn_ = x.fn_;
		w_ = x.w_;
		h_ = x.h_;
		sz_ = x.sz_;

		isKeyFrm_ = x.isKeyFrm_;
		v_ = x.v_;
		setFeatures();
	}
	return *this; // enables x=y=z;
}

void Yuv420Frm_h::resetSz(const int w, const int h)
{
	if ( w != w_ || h != h_ ) {
		w_ = w; 
		h_ = h;
		sz_ = 3 * (w*h / 2);
		isKeyFrm_ = false;
		v_.resize( sz_ );
		setFeatures();
	}
}

void Yuv420Frm_h::setFeatures()
{
	uint8_t *buf = v_.data();
	pBuf_[0] = buf; 
	pBuf_[1] = buf + w_*h_; 
	pBuf_[2] = buf + w_*h_ * 5 / 4;

	nBufSz_[0] = w_*h_; 
	nBufSz_[1] = w_*h_/4; 
	nBufSz_[2] = w_*h_/4;
}


void Yuv420Frm_h::hdCopyFrom( const Yuv420Frm_h *src )
{
	myAssert(w_ == src->w_ && h_ == src->h_, "Yuv420Frm_h::hdCopyFrom(): size does not match!");
	fn_ 		= src->fn_;
	isKeyFrm_ 	= src->isKeyFrm_;
	v_ 			= src->v_;
}

void Yuv420Frm_h::hdCopyFromBGR(const cv::Mat *srcBGR, const uint64_t fn)
{
	myAssert(w_ == srcBGR->cols && h_ == srcBGR->rows, "Yuv420Frm_h::hdCopyFromBGR(): size does not match!");

	cv::Mat yuv420;
	cv::cvtColor( *srcBGR, yuv420, cv::COLOR_BGR2YUV_I420);
	hdCopyFromBuf(yuv420.data, sz_, fn);
}

void Yuv420Frm_h::hdCopyFromBuf(const uint8_t *buf, const uint32_t bufSz, const uint64_t fn) 
{
	myAssert( bufSz == sz_, "Yuv420Frm_h::hdCopyFromBuf(): buf sz doest match, data loss!");
	memcpy(v_.data(), buf, bufSz);
	fn_ = fn;
}

void Yuv420Frm_h::hdCopyToBGR(cv::Mat *dstBGR) const
{
	const uint8_t *p0 = v_.data();
	cv::Mat picYV12 = cv::Mat(h_ * 3 / 2, w_, CV_8UC1, (void *)p0);
	cv::cvtColor(picYV12, *dstBGR, CV_YUV420p2RGB);
}

void Yuv420Frm_h::hdCopyTo(Yuv420Frm_h *des) const
{
	*des = *this;
}

void Yuv420Frm_h::hdCopyToLargerDst(Yuv420Frm_h *dst) const
{
	int wSrc = w_, hSrc = h_;
	int wDst = dst->w_;

	dst->fn_ = fn_;
	for (int ch = 0; ch < 3; ++ch) {
		if (ch > 0) {
			wSrc = w_ / 2;
			hSrc = h_ / 2;
			wDst = dst->w_ / 2;
		}

		uint8_t *pDst = dst->pBuf_[ch];
		uint8_t *pSrc = pBuf_[ch];
		for (int y = 0; y < hSrc; ++y, pDst += wDst, pSrc += wSrc) {
			memcpy(pDst, pSrc, wSrc);
		}
	}
}

void Yuv420Frm_h::dump(const std::string &folder, const std::string &tag, int roiW, int roiH, int L)
{
	char buf[1024];
	snprintf(buf, 1024, "%s/%s-fn-%09lld_L%d.png", folder.c_str(), tag.c_str(), fn_, L);
	cv::Mat bgr;
	
	hdCopyToBGR(&bgr);
	if (roiW == 0 || roiH == 0) {
		cv::imwrite(buf, bgr);
	}
	else {
		cv::Rect roi(0, 0, roiW, roiH);
		cv::Mat I = bgr(roi);
		cv::imwrite(buf, I);
	}
}

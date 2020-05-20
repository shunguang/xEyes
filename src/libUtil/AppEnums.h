#ifndef __APP_ENUMS_H__
#define __APP_ENUMS_H__

#include <stdio.h>

namespace xeyes {
	enum ThreadTaskId {
		THD_TASK_CAP = 0,
		THD_TASK_DET,
		THD_TASK_DSP,
		THD_TASK_COUNT
	};

	enum DetectionMethod {
		DET_MTD_CHG_DETECTION = 0,
		DET_MTD_DEEP_NETWORK,
		DET_MTD_COUNT
	};

	struct ImgSize {
		ImgSize(int w_=0, int h_=0) : w(w_), h(h_) {}
		ImgSize( const ImgSize &x):w(x.w), h(x.h) {}
		void set(int w_, int h_) {
			w = w_; 
			h = h_;
		}
		bool isSame(int w_, int h_) const {
			return ( (w == w_) && (h == h_) );
		}

		ImgSize getSize(int L) const {
			ImgSize sz(w >> L, h >> L);
			return sz;
		}

		int w;
		int h;
	};

	struct VideoFps {
		VideoFps(int num_ = 0, int den_ = 1) : num(num_), den(den_) {}
		void set(int num_, int den_) {
			num = num_; 
			den = den_;
		}
		bool isSame(int num_, int den_) const {
			return ( (num == num_) && (den == den_) );
		}
		float getFps() const{
			return (float)num/den;
		}
		int num;
		int den;
	};

}
#endif

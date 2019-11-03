#ifndef __CFG_RTSP_H__
#define __CFG_RTSP_H__

#include "CfgBase.h"
namespace ngv {
	struct RtspImgSzInfo {
		RtspImgSzInfo(int L_ = 0, int w1_ = 0, int h1_ = 0, int w2_ = 0, int h2_ = 0, int br_kbps_=0) : L(L_), w1(w1_), h1(h1_), w2(w2_), h2(h2_), br_kbps(br_kbps_){}
		void fromString(const std::string &s){
			std::vector<std::string> v;
			string_to_vector2( v, s, ",", false);

			assert( v.size() == 6 );
			L = atoi(v[0].c_str());
			w1 = atoi(v[1].c_str());
			h1 = atoi(v[2].c_str());
			w2 = atoi(v[3].c_str());
			h2 = atoi(v[4].c_str());
			br_kbps = atoi(v[5].c_str());
		}

		std::string toString() const {
			char buf[128];
			snprintf(buf, 128, "%d,%d,%d,%d,%d,%d", L, w1, h1, w2, h2, br_kbps);
			return std::string(buf);
		}

		int		L;		//pyramid level	
		int		w1, h1;	//actual image size from camera
		int		w2, h2;	//size used for enc and dec
		int 	br_kbps; //bitrate in kbits/s
	};

	class  CFG_EXPORT CfgRtsp : public CfgBase {
	public:
		CfgRtsp();
		CfgRtsp( const CfgRtsp &x );
		CfgRtsp& operator = (const CfgRtsp &x);
		virtual ~CfgRtsp() {};

		virtual boost::property_tree::ptree toPropertyTree();
		virtual void fromPropertyTree(const boost::property_tree::ptree &pt);
		virtual std::string toString();

		RtspImgSzInfo getRtspInfo(const int L) {
			for (RtspImgSzInfo &x : vImgSzInfo) {
				if (x.L == L) {
					return x;
				}
			}
			return RtspImgSzInfo(-1,0,0,0,0,0);
		}

	public:
		std::vector<RtspImgSzInfo> vImgSzInfo;
	};
	typedef std::shared_ptr<CfgRtsp>		CfgRtspPtr;
}

#endif

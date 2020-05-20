//config for single camera related stuff
#ifndef __CFG_CAM_H__
#define __CFG_CAM_H__

#include "CfgBase.h"
namespace xeyes {
	class  CFG_EXPORT CfgCam : public CfgBase {
	public:
		CfgCam();
		CfgCam( const CfgCam &x );
		CfgCam& operator = (const CfgCam &x);
		virtual ~CfgCam() {};

		virtual boost::property_tree::ptree toPropertyTree();
		virtual void fromPropertyTree(const boost::property_tree::ptree &pt);
		virtual std::string toString();

		uint32_t	getIp() const;
		std::string  getRecFolder() const;
	public:
		int			cameraId_;
		std::string cameraName_;
		std::string rtspUrl_;   //rtsp://192.168.1.5/11
		bool		valid_;

		ImgSize		imgSz_;	   //output image sz after H264 decoding regardless the original size from camera
		VideoFps    fps_;

		//todo: add more
		int 		frmQueSz_;      //que size for FrmInfo

		int 		detPyrLev_;    	//detetcion pyamid level
		int			detMethodId_;
		std::string detNetworkName_;
		int			detFrmsToSkip_;  //if 1 do detection every other frm, skip 2 frames after one detetcion, ....

		bool		isRec_;
		bool		isDisp_;      
		std::string mp4LocationAndPrefix_;


	};
	typedef std::shared_ptr<CfgCam>		CfgCamPtr;
}

#endif

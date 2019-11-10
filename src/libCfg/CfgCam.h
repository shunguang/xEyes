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

	public:
		int			cameraId_;
		std::string cameraName_;
		uint32_t	ip_;

		ImgSize		imgSz_;	
		VideoFps    fps_;

		//todo: add more
		int 		frmQueSz_;      //que size for FrmInfo

		int 		detPyrLev_;    	//detetcion pyamid level
		std::string rtspUrl_;
		bool		isRec_;
		bool		isDisp_;
	};
	typedef std::shared_ptr<CfgCam>		CfgCamPtr;
}

#endif

/*
*------------------------------------------------------------------------
*CfgCam.h
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
		int			detNetworkId_;
		int			detFrmsToSkip_;  //if 1 do detection every other frm, skip 2 frames after one detetcion, ....

		bool		isRec_;
		bool		isDisp_;      
		std::string mp4LocationAndPrefix_;
		std::string usrId_;
		int 		usrPw_;
		int 		maxSizeTime_;
		int			maxSizeBytes_;
		int			maxBytes_;
		int 		maxFiles_;


	};
	typedef std::shared_ptr<CfgCam>		CfgCamPtr;
}

#endif

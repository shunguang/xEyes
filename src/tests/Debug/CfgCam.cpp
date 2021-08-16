/*
*------------------------------------------------------------------------
*CfgCam.cpp
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
#include "CfgCam.h"

using namespace std;
using namespace xeyes;

CfgCam::CfgCam()
	: CfgBase()
	, cameraId_(0)
	, cameraName_("unkn")
	, rtspUrl_("unkn")
	, valid_(true)
	, imgSz_(0, 0)
	, fps_(0, 0)
	, frmQueSz_(10)
	, detPyrLev_(1)
	, detMethodId_(0)
	, detNetworkId_(0)
	, detFrmsToSkip_(2)  //if 1 do detection every other frm, skip 2 frames after one detetcion, ....
	, isRec_(true)
	, isDisp_(true)
	, mp4LocationAndPrefix_("./myRecFolder/tmp")
	, usrId_("admin")
	, usrPw_(123456)
	, maxSizeTime_(620000000)
	, maxSizeBytes_(0)
	, maxFiles_(100)
{
}

CfgCam::CfgCam( const CfgCam &x )
: CfgBase		( x )
, cameraId_		( x.cameraId_ )
, cameraName_(x.cameraName_)
, rtspUrl_		( x.rtspUrl_ )
, valid_		( x.valid_ )			
, imgSz_		( x.imgSz_ )
, fps_			( x.fps_ )
, frmQueSz_		( x.frmQueSz_ )
, detPyrLev_	( x.detPyrLev_ )
, detMethodId_	( x.detMethodId_)
, detNetworkId_ ( x.detNetworkId_)
, detFrmsToSkip_( x.detFrmsToSkip_)  //if 1 do detection every other frm, skip 2 frames after one detetcion, ....
, isRec_		( x.isRec_ )
, isDisp_		( x.isDisp_)
, mp4LocationAndPrefix_( x.mp4LocationAndPrefix_ )
, usrId_		(x.usrId_)
, usrPw_		(x.usrPw_)
, maxSizeTime_	(x.maxSizeTime_)
, maxSizeBytes_ (x.maxSizeBytes_)
, maxFiles_     (x.maxFiles_)
{
}

CfgCam& CfgCam::operator = (const CfgCam &x)
{
	//todo: m-thread unsafe
	if (this != &x){
		CfgBase::operator=(x);
		cameraId_ 	= x.cameraId_;
		cameraName_ = x.cameraName_;
		rtspUrl_	= x.rtspUrl_;
		valid_		= x.valid_ ;
		imgSz_		= x.imgSz_;
		fps_		= x.fps_;
		frmQueSz_	= x.frmQueSz_;

		detPyrLev_	=  x.detPyrLev_;
		detMethodId_ = x.detMethodId_;
		detNetworkId_ = x.detNetworkId_;
		detFrmsToSkip_ = x.detFrmsToSkip_;  //if 1 do detection every other frm, skip 2 frames after one detetcion, ....

		isRec_ = x.isRec_;
		isDisp_ = x.isDisp_;
        mp4LocationAndPrefix_ = x.mp4LocationAndPrefix_;
		usrId_ = x.usrId_;
		usrPw_ = x.usrPw_;
		maxSizeTime_ = x.maxSizeTime_;
		maxSizeBytes_ = x.maxSizeBytes_;
		maxFiles_ = x.maxFiles_;

	}
	return *this;
}

void CfgCam::fromPropertyTree(const boost::property_tree::ptree &pt)
{
	cameraId_	= pt.get<int>("cameraId");
	cameraName_ = pt.get<std::string>("cameraName");
	rtspUrl_ = pt.get<std::string>("rtspUrl");
	valid_		= pt.get<int>("valid");
	imgSz_.w 	= pt.get<int>("imgW");
	imgSz_.h 	= pt.get<int>("imgH");
	fps_.num 	= pt.get<int>("fpsNum");
	fps_.den 	= pt.get<int>("fpsDen");
	frmQueSz_	= pt.get<int>("frmQueSz");
	detPyrLev_	= pt.get<int>("detPyrLev");
	detMethodId_ = pt.get<int>("detMethodId");
	detNetworkId_ = pt.get<int>("detNetworkId");
	detFrmsToSkip_ = pt.get<int>("detFrmsToSkip");
	mp4LocationAndPrefix_ = pt.get<std::string>("mp4LocationAndPrefix");
	usrId_ = pt.get<std::string>("usrId");
	usrPw_ = pt.get<int>("usrPw");
	maxSizeTime_ = pt.get<int>("maxSizeTime");
	maxSizeBytes_ = pt.get<int>("maxSizeBytes");
	maxFiles_ = pt.get<int>("maxFiles");
}

boost::property_tree::ptree CfgCam::toPropertyTree()
{
	boost::property_tree::ptree pt;

	pt.put("cameraId", cameraId_);
	pt.put("cameraName", cameraName_);
	pt.put("rtspUrl", rtspUrl_);
	pt.put("valid", (int)valid_);
	pt.put("imgW", imgSz_.w);
	pt.put("imgH", imgSz_.h);
	pt.put("fpsNum", fps_.num);
	pt.put("fpsDen", fps_.den);
	pt.put("frmQueSz", frmQueSz_);

	pt.put("detPyrLev", detPyrLev_);
	pt.put("detMethodId", detMethodId_);
	pt.put("detNetworkId", detNetworkId_);
	pt.put("detFrmsToSkip", detFrmsToSkip_);

	pt.put( "mp4LocationAndPrefix", mp4LocationAndPrefix_);
	pt.put( "usrId", usrId_);
	pt.put( "usrPw", usrPw_);
	pt.put( "maxSizeTime", maxSizeTime_);
	pt.put( "maxSizeBytes", maxSizeBytes_);
	pt.put( "maxFiles", maxFiles_);

	return pt;
}

std::string CfgCam::toString()
{
	boost::property_tree::ptree pt = toPropertyTree();
	return CfgBase::toString(pt);
}


uint32_t  CfgCam::getIp() const 
{
	//rtspUrl_ = "rtsp://192.168.1.5/11"

	string s = rtspUrl_.substr(7);    //"192.168.1.5/11"
	vector<string> v;
	uint32_t n = string_to_vector2(v, s, "/", false);
	if (n != 2) { return 0; }

	return ipConvertStr2Num( v[0] );
}

//mp4LocationAndPrefix_ = "/home/rec/FrontDoor/rec" or ""
std::string  CfgCam::getRecFolder() const
{
	if (mp4LocationAndPrefix_.empty()){
		return "";
	}

	std::string fDir, fName;
	splitFolder(mp4LocationAndPrefix_, fDir, fName);
	return fDir;
}

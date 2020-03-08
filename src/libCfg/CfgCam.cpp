#include "CfgCam.h"

using namespace std;
using namespace xeyes;

CfgCam::CfgCam()
: CfgBase()
, cameraId_(0)
, cameraName_("unkn")
, rtspUrl_("unkn")
, valid_(true)
, imgSz_(0,0)
, fps_(0,0)
, frmQueSz_(10)
, detPyrLev_(1)
, isRec_(true)
, isDisp_(true)
, mp4LocationAndPrefix_("./myRecFolder/tmp")
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
, isRec_		( x.isRec_ )
, isDisp_		( x.isDisp_)
, mp4LocationAndPrefix_( x.mp4LocationAndPrefix_ )
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

		isRec_ = x.isRec_;
		isDisp_ = x.isDisp_;
        mp4LocationAndPrefix_ = x.mp4LocationAndPrefix_;
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
	mp4LocationAndPrefix_ = pt.get<std::string>("mp4LocationAndPrefix");
}

boost::property_tree::ptree CfgCam::toPropertyTree() 
{
	boost::property_tree::ptree pt;

	pt.put( "cameraId", 	cameraId_ );
	pt.put( "cameraName", cameraName_);
	pt.put( "rtspUrl", rtspUrl_);
	pt.put( "valid", (int)valid_);
	pt.put( "imgW", 		imgSz_.w );
	pt.put( "imgH", 		imgSz_.h );
	pt.put( "fpsNum", 		fps_.num );
	pt.put( "fpsDen", 		fps_.den );
	pt.put( "frmQueSz", 	frmQueSz_ );
	pt.put( "detPyrLev", 	detPyrLev_ );
	pt.put( "mp4LocationAndPrefix", mp4LocationAndPrefix_);
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

//mp4LocationAndPrefix_ = "/home/rec/FrontDoor/rec"
std::string  CfgCam::getRecFolder() const
{
	//mp4LocationAndPrefix_ = "/home/rec/FrontDoor/rec"
	std::string fDir, fName;
	splitFolder(mp4LocationAndPrefix_, fDir, fName);
	return fDir;
}

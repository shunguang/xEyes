#include "CfgCam.h"

using namespace std;
using namespace xeyes;

CfgCam::CfgCam()
: CfgBase()
, cameraId_(0)
, cameraName_("unkn")
, imgSz_(0,0)
, fps_(0,0)
, capQueSz_(10)
, detQueSz_(10)
, detPyrLev_(1)
, rtspUrl_("unkn")
{
}

CfgCam::CfgCam( const CfgCam &x )
: CfgBase		( x )
, cameraId_		( x.cameraId_ )
, cameraName_	( x.cameraName_ )
, imgSz_		( x.imgSz_ )
, fps_			( x.fps_ )
, capQueSz_		( x.capQueSz_ )
, detQueSz_ 	( x.detQueSz_ )
, detPyrLev_	( x.detPyrLev_ )
, rtspUrl_		( x.rtspUrl_ )
{
}

CfgCam& CfgCam::operator = (const CfgCam &x)
{
	//todo: m-thread unsafe
	if (this != &x){
		CfgBase::operator=(x);
		cameraId_ 	= x.cameraId_;
		cameraName_	= x.cameraName_;
		imgSz_		= x.imgSz_;
		fps_		= x.fps_;
		capQueSz_	= x.capQueSz_;

		detQueSz_ 	=  x.detQueSz_; 
		detPyrLev_	=  x.detPyrLev_;

		rtspUrl_	= x.rtspUrl_;;
	}
	return *this;
}

void CfgCam::fromPropertyTree(const boost::property_tree::ptree &pt)
{
	cameraId_	= pt.get<int>("cameraId");
	cameraName_	= pt.get<std::string>("cameraName");
	imgSz_.w 	= pt.get<int>("imgW");
	imgSz_.h 	= pt.get<int>("imgH");
	fps_.num 	= pt.get<int>("fpsNum");
	fps_.den 	= pt.get<int>("fpsDen");
	capQueSz_	= pt.get<int>("capQueSz");
	detQueSz_	= pt.get<int>("detQueSz");
	detPyrLev_	= pt.get<int>("detPyrLev");
	rtspUrl_ 	= pt.get<std::string>("rtspUrl");
}

boost::property_tree::ptree CfgCam::toPropertyTree() 
{
	boost::property_tree::ptree pt;

	pt.put( "cameraId", 	cameraId_ );
	pt.put( "cameraName", 	cameraName_ );
	pt.put( "imgW", 		imgSz_.w );
	pt.put( "imgH", 		imgSz_.h );
	pt.put( "fpsNum", 		fps_.num );
	pt.put( "fpsDen", 		fps_.den );
	pt.put( "capQueSz", 	capQueSz_ );
	pt.put( "detQueSz", 	detQueSz_ );
	pt.put( "detPyrLev", 	detPyrLev_ );
	pt.put( "rtspUrl", 		rtspUrl_ );
	return pt;
}

std::string CfgCam::toString()
{
	boost::property_tree::ptree pt = toPropertyTree();
	return CfgBase::toString(pt);
}

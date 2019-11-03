#include "CfgLocalView.h"

using namespace std;
using namespace xeyes;

CfgLocalView::CfgLocalView()
: CfgBase()
, minGuiWinW_(640)
, ctrlGrpBoxH_(100)
, maxGuiWinSz_(1920,1080)
, dispQueSz_(10)
, dispPyrLev_(2)
, nNumOfCams_(2)
, imgSz_L0_(1920,1080)
{
}

CfgLocalView& CfgLocalView::operator = (const CfgLocalView& x)
{
	//todo: m-thread unsafe
	if (this != &x) {
		CfgBase::operator=(x);
		maxGuiWinSz_=x.maxGuiWinSz_;
		dispQueSz_=x.dispQueSz_;
		dispPyrLev_=x.dispPyrLev_;
		nNumOfCams_=x.nNumOfCams_;
		imgSz_L0_ = x.imgSz_L0_;
		vDispPyrLev_ =  x.vDispPyrLev_;
	}
	return *this;
}


void CfgLocalView::fromPropertyTree(const boost::property_tree::ptree &pt)
{
	dispQueSz_	= pt.get<int>("dispQueSz");
	dispPyrLev_	= pt.get<int>("dispPyrLev");
}

boost::property_tree::ptree CfgLocalView::toPropertyTree() 
{
	boost::property_tree::ptree pt;

	pt.put( "dispQueSz", 	dispQueSz_ );
	pt.put( "dispPyrLev", 	dispPyrLev_ );
	return pt;
}

std::string CfgLocalView::toString()
{
	boost::property_tree::ptree pt = toPropertyTree();
	return CfgBase::toString(pt);
}


ImgSize CfgLocalView::getGuiWinSz() const
{
	int w0, h0, b = 5;
	ImgSize sz = getDispImgSz();
	if (nNumOfCams_ == 1) {
		w0 = sz.w;
		h0 = sz.h;
	}
	else if (nNumOfCams_ == 2) {
		w0 = 2*sz.w + b;
		h0 = sz.h;
	}
	else if (nNumOfCams_ == 3 || nNumOfCams_ == 4) {
		w0 = 2 * sz.w + b;
		h0 = 2 * sz.h + b;
	}
	else{
		myAssert( 0, "CfgLocalView::getGuiWinSz(): cannot supprot more than 4 cameras!");
	}

	if (w0 < minGuiWinW_) {
		w0 = minGuiWinW_;
	}
	//control pannel size
	h0 += ctrlGrpBoxH_;

	return ImgSize(w0, h0);
}

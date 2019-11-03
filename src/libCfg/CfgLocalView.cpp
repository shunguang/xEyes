#include "CfgLocalView.h"

using namespace std;
using namespace xeyes;

CfgLocalView::CfgLocalView()
: CfgBase()
, minGuiWinW_(640)
, minCtrlPanelH_(140)
, logoSz_(100,100)
, maxGuiWinSz_(1920,1080)
, dispQueSz_(10)
, dispPyrLev_(2)
, maxPyrLev_(3)
, nNumOfCams_(4)
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
		maxPyrLev_ = x.maxPyrLev_;
		nNumOfCams_=x.nNumOfCams_;
		imgSz_L0_ = x.imgSz_L0_;
		vDispPyrLev_ =  x.vDispPyrLev_;
		logoSz_ = x.logoSz_;
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
	int w0=0, h0=0, b = 2;
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
	
	w0 += 2*b;  //add extra space
	if (w0 < minGuiWinW_) {
		w0 = minGuiWinW_;
	}
	if (w0 > maxGuiWinSz_.w) {
		w0 = maxGuiWinSz_.w;
	}

	//control pannel size
	int contrlPanelH = logoSz_.h + 2 * b;
	if (contrlPanelH < minCtrlPanelH_) {
		contrlPanelH = minCtrlPanelH_;
	}
	h0 += contrlPanelH;
	if (h0 > maxGuiWinSz_.h) {
		h0 = maxGuiWinSz_.h;
	}

	return ImgSize(w0, h0);
}

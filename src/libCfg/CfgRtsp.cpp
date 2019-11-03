#include "CfgRtsp.h"

using namespace std;
using namespace ngv;

CfgRtsp::CfgRtsp()
: CfgBase()
, vImgSzInfo()
{
}

CfgRtsp::CfgRtsp( const CfgRtsp &x )
	: CfgBase( x )
	, vImgSzInfo(x.vImgSzInfo)
{
}

CfgRtsp& CfgRtsp::operator = (const CfgRtsp &x)
{
	//todo: m-thread unsafe
	assert(this != &x);

	CfgBase::operator=(x);
	vImgSzInfo = x.vImgSzInfo;
	return *this;
}


void CfgRtsp::fromPropertyTree(const boost::property_tree::ptree &pt0)
{
	vImgSzInfo.clear();
	boost::property_tree::ptree pt = pt0.get_child("imgSzTab");
	cout << pt.size() << endl;
	BOOST_FOREACH(const boost::property_tree::ptree::value_type &x, pt) {
		const string &s = x.second.data();
		RtspImgSzInfo item;
		item.fromString(s);
		vImgSzInfo.push_back(item);
	}
}

boost::property_tree::ptree CfgRtsp::toPropertyTree() 
{
	boost::property_tree::ptree pt;
	BOOST_FOREACH(const RtspImgSzInfo &item, vImgSzInfo) {
		boost::property_tree::ptree pt0;
		pt0.put("item", item.toString());
		pt.add_child("imgSzTab.item", pt0);
	}
	return pt;
}

std::string CfgRtsp::toString()
{
	boost::property_tree::ptree pt = toPropertyTree();
	return CfgBase::toString(pt);
}

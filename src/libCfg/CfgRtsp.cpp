/*
*------------------------------------------------------------------------
*CfgRtsp.cpp
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

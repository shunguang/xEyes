/*
*------------------------------------------------------------------------
*Cfg.cpp
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
#include "Cfg.h"
using namespace std;
using namespace xeyes;

Cfg::Cfg()
	: m_camIdVec()
	, m_camMap()
	, m_localView(new CfgLocalView())
	, m_log(new CfgLog())
{
}

void Cfg::readFromFile(const char *fname)
{
	boost::property_tree::ptree pt;
	boost::property_tree::xml_parser::read_xml( fname, pt);
	this->fromPropertyTree(pt.get_child("cfg"));
	printf("xml read done!\n");
}

void Cfg::writeToFile(const char *fname)
{
	boost::property_tree::ptree pt = toPropertyTree();
	boost::property_tree::xml_parser::xml_writer_settings<std::string> settings(' ', 4);
	boost::property_tree::xml_parser::write_xml(fname, pt, std::locale(), settings);
}

std::string Cfg::toString()
{
	boost::property_tree::ptree pt = toPropertyTree();

	std::ostringstream oss;
	boost::property_tree::xml_parser::xml_writer_settings<std::string> settings(' ', 4);
	boost::property_tree::xml_parser::write_xml(oss, pt, settings);
	return oss.str();
}

void Cfg::fromPropertyTree(const boost::property_tree::ptree &pt0)
{
	const boost::property_tree::ptree pt = pt0.get_child("cams");
	m_camIdVec.clear();
  BOOST_FOREACH(const boost::property_tree::ptree::value_type &g, pt) {
		CfgCamPtr curr( new CfgCam() );
		curr->fromPropertyTree( g.second );
		//cout << curr->toString() << endl;
		if( curr->valid_ ){
			int camId = curr->cameraId_;
			m_camMap[camId] = curr; 
			m_camIdVec.push_back( camId );
		}
	}
	m_localView->fromPropertyTree(pt0.get_child("disp"));
	m_localView->nNumOfCams_ = m_camIdVec.size();
	m_log->fromPropertyTree(pt0.get_child("log"));
}


boost::property_tree::ptree Cfg::toPropertyTree()
{
	boost::property_tree::ptree ptLog = m_log->toPropertyTree();
	boost::property_tree::ptree ptDisp = m_localView->toPropertyTree();
	boost::property_tree::ptree pt;
	for ( const auto camId : m_camIdVec){
		CfgCam cam = getCam( camId );
		boost::property_tree::ptree  ptCam = cam.toPropertyTree();
		pt.add_child( "cfg.cams.cam", ptCam );
	}	

	pt.add_child("cfg.disp", ptDisp);
	pt.add_child("cfg.log", ptLog);

	return pt;
}


int Cfg::increaseDispImgPyrL() 
{
	int L;
	{
		boost::mutex::scoped_lock lock(m_mutex);
		L = m_localView->dispPyrLev_ + 1;
		if (L > m_localView->vDispPyrLev_.back()) {
			L = m_localView->vDispPyrLev_.back();
		}
	}
	return L;
}


void Cfg::updateRecFlag(int camIdx, bool isRecording) {
	boost::mutex::scoped_lock lock(m_mutex);
	m_camMap[camIdx]->isRec_ = isRecording;
}

void Cfg::updateDispFlag(int camIdx, bool isDisp) {
	boost::mutex::scoped_lock lock(m_mutex);
	m_camMap[camIdx]->isDisp_ = isDisp;
}

void Cfg::updateCamName(int camIdx, std::string name)
{
	boost::mutex::scoped_lock lock(m_mutex);
	m_camMap[camIdx]->cameraName_ = name;
}

cv::Size Cfg::getDspImgSz(int camId)
{
	CfgCam camCfg = getCam( camId );
	CfgLocalView lv = getLocalView();

	const int &w0 = camCfg.imgSz_.w;
	const int &h0 = camCfg.imgSz_.h;
	cv::Size dspSz( w0 >> lv.dispPyrLev_, h0 >> lv.dispPyrLev_);
	return dspSz;
}

/*
*------------------------------------------------------------------------
*Cfg.h
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
#ifndef __CFG_H__
#define __CFG_H__

#include "CfgLog.h"
#include "CfgCam.h"
#include "CfgLocalView.h"
#include "CfgDefs.h"
namespace xeyes {
	class CFG_EXPORT Cfg {
	public:
		Cfg();

		void readFromFile(const char *fname);
		void writeToFile(const char *fname);
		std::string toString();

		CfgLog getLog() {
			CfgLog ret; 
			{
				boost::mutex::scoped_lock lock(m_mutex);
				ret = *(m_log.get());
			}
			return ret;
		}
		CfgLocalView getLocalView() {
			CfgLocalView ret; 
			{
				boost::mutex::scoped_lock lock(m_mutex);
				ret = *(m_localView.get());
			}
			return ret;
		}

		//return the camerId vector
		void getCamIds( std::vector<int> &v ) {
			boost::mutex::scoped_lock lock( m_mutex );
			v = m_camIdVec;
		}

		//access camera info by camId
		CfgCam getCam( const int camId ) {
			CfgCam ret;
			{
				boost::mutex::scoped_lock lock( m_mutex );
				ret = *(m_camMap[camId].get());
			}
			return ret;
		}

		cv::Size getDspImgSz(int camId);

		//<camIdx> must start from 0 for the flowing 3 funcs;
		void updateRecFlag(int camIdx, bool isRecording);
		void updateDispFlag(int camIdx, bool isDisp);
		void updateCamName(int camIdx, std::string name);

		int increaseDispImgPyrL();
		void updateDispImgPryLevel(const int newL) {
			boost::mutex::scoped_lock lock(m_mutex);
			m_localView->dispPyrLev_ = newL ;
		}


	private:
		boost::property_tree::ptree toPropertyTree();
		void fromPropertyTree(const boost::property_tree::ptree &pt) ;

	protected:
		std::vector<int>			m_camIdVec;     //registered camera IDs
		std::map<int, CfgCamPtr>    m_camMap;   	//<cameraId, camCfg> map
		CfgLocalViewPtr				m_localView;   		//disp
		CfgLogPtr					m_log;   		//log
		boost::mutex				m_mutex;
	};

	typedef std::shared_ptr<Cfg>		CfgPtr;
}
#endif

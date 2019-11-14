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

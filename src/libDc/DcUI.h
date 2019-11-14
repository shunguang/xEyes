//----------------------------------------------------------------------------------------
//Shared data container interface for xEye system, typically holds multiple cameras
//----------------------------------------------------------------------------------------
#ifndef _DC_UI_H_
#define _DC_UI_H_

#include "libCfg/Cfg.h"
#include "Dc.h"
namespace xeyes {
	class DS_EXPORT DcUI{
	public:
		DcUI( CfgPtr &cfg );
		DcUI( const DcUI &x) = delete;
		DcUI& operator=(DcUI const&) = delete;
		~DcUI() = default;
		
	protected:
		//initial shared data container
		void initSharedDc();
		
	public:
		CfgPtr 					m_cfg;
		int						m_nTotCams; 
		std::map<int, DcPtr> 	m_dcMap;          //assert(m_dcMap.size() == m_nTotCams )
	};
	typedef std::shared_ptr<DcUI> DcUIPtr;
}

#endif		
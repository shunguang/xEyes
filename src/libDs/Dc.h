//----------------------------------------------------------------------------------------
//data container for one camera, shared by all threads
//----------------------------------------------------------------------------------------
#ifndef _DC_H_
#define _DC_H_

#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "YuvCircularQ_h.h"
#include "RgbCircularQ_h.h"

namespace xeyes {
	class DS_EXPORT Dc{
	public:
		Dc( const int camId,  CfgPtr &cfg );
		Dc( const Dc &x ) = delete;
		Dc& operator=(Dc const&) = delete;
		~Dc() = default;

	public:
		int 				m_camId;
		CfgPtr 				m_cfg;
		YuvCircularQ_h		m_capYuvFrmQ;       //capture results
		RgbCircularQ_h		m_detRgbFrmQ;       //detection results
	};
	typedef std::shared_ptr<Dc> DcPtr;
}

#endif		
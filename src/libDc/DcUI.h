/*
*------------------------------------------------------------------------
*DcUI.h
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

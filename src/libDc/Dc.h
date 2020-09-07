/*
*------------------------------------------------------------------------
*Dc.h
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
//data container for one camera, shared by all threads
//----------------------------------------------------------------------------------------
#ifndef _DC_H_
#define _DC_H_

#include "libUtil/util.h"
#include "libCfg/Cfg.h"
#include "YuvCircularQ_h.h"
#include "RgbCircularQ_h.h"
#include "FrmInfoCircularQ.h"

namespace xeyes {
	class DS_EXPORT Dc{
	public:
		Dc( const int camId,  CfgPtr &cfg );
		Dc( const Dc &x ) = delete;
		Dc& operator=(Dc const&) = delete;
		~Dc() = default;

	public:
		int 					m_camId;
		CfgPtr 					m_cfg;
		FrmInfoCircularQPtr		m_frmInfoQ;       //capture results
	};
	typedef std::shared_ptr<Dc> DcPtr;
}

#endif		

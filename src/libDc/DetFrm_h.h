/*
*------------------------------------------------------------------------
*DetFrm_h.h
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
#ifndef __DET_FRM_H_H__
#define __DET_FRM_H_H__
#include "libUtil/util.h"
#include "RgbFrm_h.h"
#include "DsDefs.h"
namespace xeyes {
	class DS_EXPORT DetFrm_h{
	public:
		DetFrm_h( const int w0=0, const int h0=0, const int L=0, const uint64_t fn=0);
		DetFrm_h( const DetFrm_h &x);
		~DetFrm_h() = default;
		DetFrm_h& operator = (const DetFrm_h &x);

		void hdCopyFrom( const DetFrm_h *src) {
			m_L = src->m_L;
			m_fn = src->m_fn;
			m_vRois = src->m_vRois;
			m_rgbImg.hdCopyFrom( &src->m_rgbImg);
		}

		void hdCopyTo(DetFrm_h *dst) const {
			dst->m_L = m_L;
			dst->m_fn = m_fn;
			dst->m_vRois = m_vRois;
			m_rgbImg.hdCopyTo( &dst->m_rgbImg ) ;
		}

		void resetSz(int w0, int h0, int  L) {
			m_L = L;
			m_fn=0;
			m_vRois.clear();
			m_rgbImg.resize(w0>>L, h0>>L, L);
		}

		void dump(const std::string &folderPath, const std::string &tag) const;

	public:
        int		    		m_L;            //detetcion pyramid level 
		uint64_t			m_fn;			//frm # in capture
        std::vector<Roi>  	m_vRois;		//Rois in the frm
		RgbFrm_h			m_rgbImg;
	};
	typedef std::shared_ptr<DetFrm_h>		DetFrm_hPtr;
}  

#endif		

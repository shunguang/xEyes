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
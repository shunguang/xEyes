#ifndef __DET_FRM_H_H__
#define __DET_FRM_H_H__
#include "libUtil/util.h"
#include "DsDefs.h"
namespace xeyes {
	class DS_EXPORT DetFrm_h{
	public:
		DetFrm_h( const int L=0, const uint64_t fn=0);
		DetFrm_h( const DetFrm_h &x) = default;
		~DetFrm_h() = default;
		DetFrm_h& operator = (const DetFrm_h &x) = default;

		void hdCopyFrom( const DetFrm_h *src) {
			m_L = src->m_L;
			m_fn = src->m_fn;
			m_vRois = src->m_vRois;
		}

		void hdCopyTo(DetFrm_h *dst) const {
			dst->m_L = m_L;
			dst->m_fn = m_fn;
			dst->m_vRois = m_vRois;
		}

		void resetL(int  L) {
			m_L = L;
		}

		void dump(const std::string &folderPath) const;

	public:
        int		    		m_L;            //detetcion pyramid level 
		uint64_t			m_fn;			//frm # in capture
        std::vector<Roi>  	m_vRois;		//Rois in the frm
	};
	typedef std::shared_ptr<DetFrm_h>		DetFrm_hPtr;
}  

#endif		
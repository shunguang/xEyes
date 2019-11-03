#ifndef __DET_FRM_H_H__
#define __DET_FRM_H_H__
#include "libUtil/util.h"
#include "DsDefs.h"
namespace xeyes {
	class DS_EXPORT DetFrm_h{
	public:
		DetFrm_h( const int camId=0, const int L=0, const uint64_t fn=0);
		DetFrm_h( const DetFrm_h &x) = default;
		~DetFrm_h() = default;
		DetFrm_h& operator = (const DetFrm_h &x) = default;

		void dump(const std::string &folderPath) const;

	public:
        int         		m_camId;
        int		    		m_L;            //detetcion pyramid level 
		uint64_t			m_fn;			//frm # in capture
        std::vector<Roi>  	m_vRois;		//Rois in the frm
	};
	typedef std::shared_ptr<DetFrm_h>		DetFrm_hPtr;
}  

#endif		
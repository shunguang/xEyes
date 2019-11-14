#ifndef __DISP_FRM_H_H__
#define __DISP_FRM_H_H__
#include <QtGui/QPixmap>
#include "libUtil/util.h"
#include "DsDefs.h"
namespace xeyes {
	class DS_EXPORT DspFrm_h{
	public:
		DspFrm_h(int w, int h);
		DspFrm_h( const DspFrm_h &x);
		~DspFrm_h();
		DspFrm_h& operator = (const DspFrm_h &x);


		void hdCopyFrom( const DspFrm_h *src ) {
			m_fn = src->m_fn;
			m_img = src->m_img.copy();
		}

		void hdCopyTo(DspFrm_h *dst ) const {
			dst->m_fn = m_fn;
			dst->m_img = m_img.copy();
		}

		void resetSz(int w, int h) {
			m_img= QPixmap(w,h);
		}

		void dump(const std::string &folderPath) const;

	public:
		uint64_t	m_fn;			//frm # in stream
		uint64_t    m_keyFrmIdx;	//key frm idx in stream
		uint64_t    m_timeStamp_ms; //time stamp at the frm in ms
		QPixmap		m_img;			//QBitMap used for disp img on device
	};
	typedef std::shared_ptr<DspFrm_h>		DspFrm_hPtr;
}  

#endif		
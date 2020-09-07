/*
*------------------------------------------------------------------------
*DspFrm_h.h
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
#ifndef __DISP_FRM_H_H__
#define __DISP_FRM_H_H__
#include <QtGui/QPixmap>
#include "libUtil/util.h"
#include "DsDefs.h"

#define DSP_USE_QPIXMAP 1
namespace xeyes {
	class DS_EXPORT DspFrm_h{
	public:
		DspFrm_h(int w, int h);
		DspFrm_h( const DspFrm_h &x);
		~DspFrm_h();
		DspFrm_h& operator = (const DspFrm_h &x);


		void hdCopyFrom( const DspFrm_h *src ) {
			m_fn = src->m_fn;
#if DSP_USE_QPIXMAP			
			m_img = src->m_img.copy();
#endif
		}

		void hdCopyTo(DspFrm_h *dst ) const {
			dst->m_fn = m_fn;
#if DSP_USE_QPIXMAP			
			dst->m_img = m_img.copy();
#endif
		}

		void resetSz(int w, int h) {
#if DSP_USE_QPIXMAP			
			m_img= QPixmap(w,h);
#endif
		}

		void dump(const std::string &folderPath) const;

	public:
		uint64_t	m_fn;			//frm # in stream
		uint64_t    m_keyFrmIdx;	//key frm idx in stream
		uint64_t    m_timeStamp_ms; //time stamp at the frm in ms
#if DSP_USE_QPIXMAP			
		QPixmap		m_img;			//QBitMap used for disp img on device
#endif
	};
	typedef std::shared_ptr<DspFrm_h>		DspFrm_hPtr;
}  

#endif		

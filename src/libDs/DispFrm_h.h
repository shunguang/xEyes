#ifndef __DISP_FRM_H_H__
#define __DISP_FRM_H_H__
#include <QtGui/QImage>
#include "libUtil/util.h"
#include "Yuv420Frm_h.h"
#include "DsDefs.h"
namespace xeyes {
	class DS_EXPORT DispFrm_h{
	public:
		DispFrm_h(int w, int h);
		DispFrm_h( const DispFrm_h &x);
		~DispFrm_h();
		DispFrm_h& operator = (const DispFrm_h &x);

		void hdcpyFromYuvFrm(const Yuv420Frm_hPtr &frm);

		void hdcpyFrom( const QImage &I, uint64_t fn) {
			m_img = I.copy();
			m_fn = fn;
		}

		void copyTo(DispFrm_h &dst ) const {
			dst.m_fn = m_fn;
			dst.m_img = m_img.copy();
		}

		void resetSz(int w, int h) {
			//m_img = m_img.scaled(w, h);
			m_cvImg.create(h, w, cv::8UC3);
		}

		void dump(const std::string &folderPath) const;

	public:
		uint64_t	m_fn;			//frm # in file
		uint64_t    m_keyFrmIdx;	//key frm idx in file
		uint64_t    m_timeStamp_ms; //time stamp at the frm in ms
		//QImage	m_qimg;			//m x n x k (k=1,3) 8-bit image, CV_8UC1 or CV_8UC3 type image to display
		cv::Mat     m_cvImg; 
	};
	typedef std::shared_ptr<DispFrm_h>		DispFrm_hPtr;
}  

#endif		